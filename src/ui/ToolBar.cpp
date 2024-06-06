#include "ToolBar.h"
#include "common/Theme.h"
#include "Desktop.h"
#include "SettingsManager.h"
#include "resources/ResourceBrowser.h"
#include "game-services/GameServiceInspector.h"
#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "game-modes/GameModeInspector.h"
#include "core-services/GameUpdaterInspector.h"
#include "core-services/GraphicsContextInspector.h"
#include "core-services/RenderManagerInspector.h"
#include "core-services/RenderingEngineInspector.h"
#include "core-services/CameraManagerInspector.h"
#include "core-services/MemoryInspector.h"
#include "tools/NeedleFxSceneDataTester.h"
#include "tools/NeedleFxSceneDataTesterV2.h"
#endif
#include "tools/RflComparer.h"
#include <debug-rendering/GOCVisualDebugDrawRenderer.h>

using namespace hh::game;

void ToolBar::Render() {
	ImGuiWindowFlags windowFlags
		= ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoCollapse;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 0));

	if (!ImGui::Begin("Sonic Forces Devtools", NULL, windowFlags)) {
		ImGui::End();
		return;
	}

	if (ImGui::BeginMenuBar()) {
		if (ImGui::MenuItem("Resource Browser"))
			new (Desktop::instance->GetAllocator()) ResourceBrowser(Desktop::instance->GetAllocator());

		if (ImGui::BeginMenu("Inspectors")) {
			if (ImGui::MenuItem("GameService"))
				new (Desktop::instance->GetAllocator()) GameServiceInspector(Desktop::instance->GetAllocator());
#ifdef DEVTOOLS_TARGET_SDK_rangers
			if (ImGui::MenuItem("GameUpdater"))
				new (Desktop::instance->GetAllocator()) GameUpdaterInspector(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("GraphicsContext"))
				new (Desktop::instance->GetAllocator()) GraphicsContextInspector(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("RenderManager"))
				new (Desktop::instance->GetAllocator()) RenderManagerInspector(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("RenderingEngine"))
				new (Desktop::instance->GetAllocator()) RenderingEngineInspector(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("Memory"))
				new (Desktop::instance->GetAllocator()) MemoryInspector(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("GameMode"))
				new (Desktop::instance->GetAllocator()) GameModeInspector(Desktop::instance->GetAllocator());
#endif
			ImGui::EndMenu();
		}

#ifdef DEVTOOLS_TARGET_SDK_rangers
		if (ImGui::BeginMenu("Tools")) {
			if (ImGui::MenuItem("NeedleFxSceneData Tester") && ImGui::FindWindowByName("NeedleFxSceneData testing tool") == nullptr)
				new (Desktop::instance->GetAllocator()) NeedleFxSceneDataTester(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("NeedleFxSceneData Tester V2") && ImGui::FindWindowByName("NeedleFxSceneData testing tool V2") == nullptr)
				new (Desktop::instance->GetAllocator()) NeedleFxSceneDataTesterV2(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("RFL Comparer"))
				new (Desktop::instance->GetAllocator()) RflComparer(Desktop::instance->GetAllocator());
			ImGui::EndMenu();
		}
#endif

		if (ImGui::BeginMenu("Mode")) {
			if (ImGui::MenuItem("Object Inspection"))
				Desktop::instance->SwitchToObjectInspectionMode();
			if (ImGui::MenuItem("Level Editor"))
				Desktop::instance->SwitchToLevelEditorMode();
#ifdef DEVTOOLS_TARGET_SDK_rangers
			if (ImGui::MenuItem("SurfRide Editor"))
				Desktop::instance->SwitchToSurfRideEditorMode();
#endif
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Settings"))
			SettingsManager::OpenConfigDialog();

		ImGui::EndMenuBar();
	}

#ifdef DEVTOOLS_TARGET_SDK_rangers
	auto& gameUpdater = GameApplication::GetInstance()->GetGameUpdater();

	unsigned int gameUpdaterFlags = static_cast<unsigned int>(gameUpdater.flags.m_dummy);

	ImGui::CheckboxFlags("Object pause", &gameUpdaterFlags, 1 << static_cast<uint8_t>(GameUpdater::Flags::OBJECT_PAUSE));
	ImGui::SetItemTooltip("Pauses only standard object layers. Certain layers and services (e.g. weather service) keep running. (Hotkey: F3)");
	ImGui::SameLine();
	ImGui::CheckboxFlags("Debug pause", &gameUpdaterFlags, 1 << static_cast<uint8_t>(GameUpdater::Flags::DEBUG_PAUSE));
	ImGui::SetItemTooltip("Pauses (almost) every object layer. (Hotkey: F4)");
	ImGui::SameLine();

	gameUpdater.flags.m_dummy = gameUpdaterFlags;

	if (ImGui::Button("Step frame"))
		gameUpdater.flags.set(GameUpdater::Flags::DEBUG_STEP_FRAME);
	ImGui::SetItemTooltip("When in debug pause, step through updates frame by frame. Note that if object pause is also enabled objects will not update even while stepping. (Hotkey: F5)");
	ImGui::SameLine();

	if (ImGui::IsKeyPressed(ImGuiKey_F3))
		gameUpdater.flags.flip(GameUpdater::Flags::OBJECT_PAUSE);
	if (ImGui::IsKeyPressed(ImGuiKey_F4))
		gameUpdater.flags.flip(GameUpdater::Flags::DEBUG_PAUSE);
	if (ImGui::IsKeyPressed(ImGuiKey_F5))
		gameUpdater.flags.set(GameUpdater::Flags::DEBUG_STEP_FRAME);
#endif

	auto* debugCameraMgr = DebugCameraManager::GetInstance();

	bool debugCameraActive{ debugCameraMgr->isActive };

	ImGui::Checkbox("Debug camera", &debugCameraActive);
	ImGui::SetItemTooltip(
		"Enable the debug (free) camera. (F6)\n"
		"\n"
		"While in debug camera mode, all inputs are intercepted and the camera can be controlled using these key bindings:\n"
		"\n"
		"Gamepad:\n"
		"Left stick/dpad: move camera\n"
		"Right stick: rotate camera\n"
		"Hold left trigger + left stick/dpad up/down: move camera up/down\n"
		"Hold left trigger + right stick/dpad up/down: zoom in/out\n"
		"Hold left shoulder + right stick left/right: roll camera\n"
		"Hold right shoulder + right stick up/down: change fov\n"
		"X: reset camera\n"
		"Y: change camera movement speed\n"
		"B: change viewport\n"
		"\n"
		"Keyboard + mouse:\n"
		"WASD / arrow keys: move camera\n"
		"Mouse / numpad arrow keys: rotate camera\n"
		"Hold LCTRL/RCTRL + move forward backward OR PgUp/PgDn: move camera up/down\n"
		"Keypad +/-: zoom in/out\n"
		"Q/E: roll camera\n"
		"Home/End: change fov\n"
		"F1: reset camera\n"
		"LShift/RShift: toggle camera movement speed\n"
		"Keypad asterisk: change viewport"
	);
	ImGui::SameLine();

	if (ImGui::IsKeyPressed(ImGuiKey_F6))
		debugCameraActive = !debugCameraActive;

	if (debugCameraMgr->isActive != debugCameraActive) {
		if (debugCameraActive)
			debugCameraMgr->ActivateDebugCamera({ 0, 0, 1, 0 });
		else
			debugCameraMgr->DeactivateDebugCamera();
	}

	if (debugCameraActive) {
		bool debugCameraLocked{ debugCameraMgr->GetCamera()->IsLocked() };
		bool tempDebugCameraLocked{ debugCameraLocked };

		ImGui::Checkbox("Lock debug camera position", &tempDebugCameraLocked);

		if (debugCameraLocked != tempDebugCameraLocked)
			debugCameraMgr->GetCamera()->SetLocked(tempDebugCameraLocked);

		ImGui::SameLine();
	}

	ImGui::Checkbox("Render debug visuals", &GOCVisualDebugDrawRenderer::instance->enabled);

	ImGui::End();
}
