#include "Pch.h"
#include "ToolBar.h"
#include "common/Theme.h"
#include "Desktop.h"
#include "ResourceBrowser.h"
#include "SettingsManager.h"
#include "core-services/GameUpdaterInspector.h"
#include "core-services/GraphicsContextInspector.h"
#include "core-services/CameraManagerInspector.h"

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

	if (!ImGui::Begin("Sonic Frontiers Devtools", NULL, windowFlags)) {
		ImGui::End();
		return;
	}

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Game")) {
			ImGui::MenuItem("Switch GameMode...");
			ImGui::MenuItem("Load Level...");
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Resource Browser"))
			new (Desktop::instance->GetAllocator()) ResourceBrowser(Desktop::instance->GetAllocator());

		if (ImGui::BeginMenu("Insights")) {
			if (ImGui::MenuItem("GameUpdater"))
				new (Desktop::instance->GetAllocator()) GameUpdaterInspector(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("GraphicsContext"))
				new (Desktop::instance->GetAllocator()) GraphicsContextInspector(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("CameraManager"))
				new (Desktop::instance->GetAllocator()) CameraManagerInspector(Desktop::instance->GetAllocator());
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Mode")) {
			ImGui::MenuItem("Object Inspection");
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Settings"))
			SettingsManager::OpenConfigDialog();

		ImGui::EndMenuBar();
	}

	auto& gameUpdater = GameApplication::GetInstance()->GetGameUpdater();

	unsigned int gameUpdaterFlags = static_cast<unsigned int>(gameUpdater.flags.m_dummy);

	ImGui::CheckboxFlags("Object pause", &gameUpdaterFlags, 1 << static_cast<uint8_t>(GameUpdater::Flags::OBJECT_PAUSE));
	ImGui::SetItemTooltip("Pauses only standard object layers. Certain layers and services (e.g. weather service) keep running. (Hotkey: F3)");
	ImGui::SameLine();
	ImGui::CheckboxFlags("Debug pause", &gameUpdaterFlags, 1 << static_cast<uint8_t>(GameUpdater::Flags::DEBUG_PAUSE));
	ImGui::SetItemTooltip("Pauses (almost) every object layer. (Hotkey: F4)");
	ImGui::SameLine();

	gameUpdater.flags.m_dummy = static_cast<GameUpdater::Flags>(gameUpdaterFlags);

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
		"Keyboard (these are a bit clunky, will improve in a future version):\n"
		"Arrow keys: move camera\n"
		"Numpad arrow keys: rotate camera\n"
		"Hold 4 on number row + arrow keys: move camera up/down\n"
		"Hold 4 on number row + numpad up/down: zoom in/out\n"
		"Hold 5 on number row + numpad left/right: roll camera\n"
		"Hold 6 on number row + numpad up/down: change fov\n"
		"2 on number row: reset camera\n"
		"3 on number row: change camera movement speed\n"
		"7 on number row: change viewport"
	);

	if (ImGui::IsKeyPressed(ImGuiKey_F6))
		debugCameraActive = !debugCameraActive;

	if (debugCameraMgr->isActive != debugCameraActive) {
		if (debugCameraActive)
			debugCameraMgr->ActivateDebugCamera({ 0, 0, 1, 0 });
		else
			debugCameraMgr->DeactivateDebugCamera();
	}

	ImGui::End();
}
