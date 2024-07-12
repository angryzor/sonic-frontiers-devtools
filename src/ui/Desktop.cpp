#include "common/Icons.h"
#include "Desktop.h"
#include "ToolBar.h"
#include "SettingsManager.h"
#include "resources/ResourceBrowser.h"
#include "operation-modes/modes/object-inspection/ObjectInspection.h"
#include "operation-modes/modes/level-editor/LevelEditor.h"
#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "operation-modes/modes/surfride-editor/SurfRideEditor.h"
#endif
#include "reflection/serialization/ReflectionSerializer.h"
#include <utilities/math/MathUtils.h>
#include <utilities/CompatibleObject.h>
#include "common/editors/Basic.h"
#include "common/inputs/Basic.h"

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::physics;

Desktop* Desktop::instance{};
bool Desktop::selectionColliderFilters[32][32]{ true };

Desktop::Desktop(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator }
{
	resourceLoader = hh::fnd::ResourceLoader::Create(allocator);
	resourceLoader->LoadPackfile("mods/angryzor_devtools/devtools.pac", 0);

	Translations::Init(allocator);
}

namespace app::game {
	class GrindService : hh::game::GameService {
		GAMESERVICE_CLASS_DECLARATION(GrindService)
	};
}
void Desktop::Render() {
	RenderOverlayWindow();

	ToolBar::Render();
	operationMode->Render();
	ResourceBrowser::RenderDialogs();

	csl::ut::MoveArray<StandaloneWindow*> windowsThatWantToClose{ hh::fnd::MemoryRouter::GetTempAllocator() };

	for (auto& window : windows)
		if (!window->Render())
			windowsThatWantToClose.push_back(window);

	for (auto& window : windowsThatOpened)
		AddStandaloneWindow(window);

	windowsThatOpened.clear();

	for (auto* window : windowsThatWantToClose)
		RemoveStandaloneWindow(window);

	SettingsManager::Render();

	HandleShortcuts();

	//if (auto* s = GameManager::GetInstance()->GetService<app::game::GrindService>()) {
	//	ImGui::Text("%d", *reinterpret_cast<bool*>(reinterpret_cast<size_t>(s) + 0x1d0));
	//	ImGui::Checkbox("grind", reinterpret_cast<bool*>(reinterpret_cast<size_t>(s) + 0x1d0));
	//}
	//static char packfileName[300];
	//ImGui::InputText("Packfile", packfileName, sizeof(packfileName));
	//if (ImGui::Button("Put packfile in objectworld lol"))
	//	if (auto* objectWorld = GameManager::GetInstance()->GetService<hh::game::ObjectWorld>())
	//		objectWorld->packFile = ResourceManager::GetInstance()->GetResource<hh::fnd::Packfile>(packfileName);

	//if (ImGui::Button("Add island objinfo")) {

	//	auto* moduleAllocator = hh::fnd::MemoryRouter::GetModuleAllocator();
	//	auto* islandObjInfo = new (moduleAllocator) app::IslandObjInfo(moduleAllocator);
	//	GameManager::GetInstance()->GetService<ObjInfoContainer>()->Register(islandObjInfo->GetInfoName(), islandObjInfo);

	//}

	//if (ImGui::Button("Viewport")) {
	//	if (auto* fxParamMgr = GameManager::GetInstance()->GetService<app::gfx::FxParamManager>()) {
	//		auto& stageConfig = fxParamMgr->sceneParameters[fxParamMgr->currentSceneParameters]->sceneData->stageConfig;
	//		if (auto* camSrv = GameManager::GetInstance()->GetService<app::camera::CameraService>()) {
	//			camSrv->CreateDefaultCameraFrame(2, 1, 0, stageConfig.camera.zNear, stageConfig.camera.zFar, stageConfig.camera.fovy);
	//		}
	//	}
	//}



	//	auto* gameManager = GameManager::GetInstance();

	//	for (uint8_t i = 0; i < 4; i++) {
	//		if (ImGui::Selectable(charaNames[i])) {
	//			if (auto* levelInfo = gameManager->GetService<app::level::LevelInfo>()) {
	//				if (auto* fxParamMgr = gameManager->GetService<app::gfx::FxParamManager>()) {
	//					if (levelInfo->stageData->attributeFlags.test(charaAttrFlags[i])) {
	//						app::player::Player::Kill(gameManager, 1);
	//						app::player::Player::PlayerSetupInfo psi;
	//						psi.playerId = 1;
	//						psi.characterId = static_cast<app::player::CharacterIdU8>(i);
	//						psi.worldPos.m_Position = pickedLocation;
	//						psi.worldPos.m_Rotation = csl::math::Quaternion::Identity;
	//						psi.deadFallTime = fxParamMgr->sceneParameters[fxParamMgr->currentSceneParameters]->sceneData->stageConfig.common.deadFallTime;
	//						psi.deadline = fxParamMgr->sceneParameters[fxParamMgr->currentSceneParameters]->sceneData->stageConfig.common.deadline;
	//						psi.oceanSurface = fxParamMgr->sceneParameters[fxParamMgr->currentSceneParameters]->sceneData->stageConfig.common.oceanSurface;
	//						psi.unk6 = 1;
	//						psi.startType = app::player::Player::StartType::STAND;
	//						app::player::Player::Spawn(gameManager, psi);
	//					}
	//				}
	//			}
	//			ImGui::CloseCurrentPopup();
	//		}
	//	}
	//	ImGui::EndPopup();
	//}
}

void Desktop::RenderOverlayWindow()
{
	auto* ivp = ImGui::GetMainViewport();

	ImGui::SetNextWindowSize(ivp->Size);
	ImGui::SetNextWindowPos(ivp->Pos);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
	ImGui::PushStyleColor(ImGuiCol_Border, 0);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor(3);
}

void Desktop::AddStandaloneWindow(StandaloneWindow* window) {
	this->windows.push_back(hh::fnd::Reference<StandaloneWindow>{ window });
}

void Desktop::RemoveStandaloneWindow(StandaloneWindow* window) {
	this->windows.remove(this->windows.find(window));
}

void Desktop::OpenStandaloneWindow(StandaloneWindow* window) {
	this->windowsThatOpened.push_back(hh::fnd::Reference<StandaloneWindow>{ window });
}

Desktop::~Desktop()
{
	delete operationMode;
}

void Desktop::Dispatch(const ActionBase& action)
{
	operationMode->ProcessAction(action);
}

void Desktop::UnbindShortcut(ShortcutId shortcutId)
{
	for (auto i = 0; i < boundShortcuts.size(); i++)
		if (boundShortcuts[i].shortcutId == shortcutId) {
			boundShortcuts.remove(i);
			break;
		}
}

void Desktop::HandleShortcuts()
{
	for (auto& boundShortcut : boundShortcuts) {
		auto& keyCombo = GetShortcutKeyCombo(boundShortcut.shortcutId);

		if ((keyCombo.modifiers & ShortcutModifier_Alt) && !ImGui::IsKeyDown(ImGuiKey_LeftAlt) && !ImGui::IsKeyDown(ImGuiKey_RightAlt))
			continue;

		if ((keyCombo.modifiers & ShortcutModifier_Ctrl) && !ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && !ImGui::IsKeyDown(ImGuiKey_RightCtrl))
			continue;

		if ((keyCombo.modifiers & ShortcutModifier_Shift) && !ImGui::IsKeyDown(ImGuiKey_LeftShift) && !ImGui::IsKeyDown(ImGuiKey_RightShift))
			continue;

		if (ImGui::IsKeyPressed(keyCombo.key))
			Dispatch(ActionBase{ boundShortcut.actionId });
	}
}
