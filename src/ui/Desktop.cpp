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
	//HandleMousePicking();

	ToolBar::Render();
	operationMode->RenderOperationMode();
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

	//if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && locationPicked) {
	//	ImGui::OpenPopup("WorldContext");
	//}

	//if (ImGui::BeginPopup("WorldContext")) {
	//	if (ImGui::Selectable("Teleport player")) {
	//		if (auto* levelInfo = hh::game::GameManager::GetInstance()->GetService<app::level::LevelInfo>())
	//		if (auto* player = static_cast<app::player::Player*>(hh::fnd::MessageManager::GetInstance()->GetMessengerByHandle(levelInfo->GetPlayerObject(0))))
	//		if (auto* playerKine = player->GetComponent<app::player::GOCPlayerKinematicParams>()) {
	//			playerKine->SetPosition({ pickedLocation.x(), pickedLocation.y(), pickedLocation.z(), 0.0f });
	//		}
	//	}
	//	ImGui::EndPopup();
	//};

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

	//if (ImGui::Button("Export")) {
	//	auto* sonicRfl = hh::fnd::ResourceManager::GetInstance()->GetResource<hh::fnd::ResReflection<app::rfl::SonicParameters>>("player_common");
	//	auto* amyRfl = hh::fnd::ResourceManager::GetInstance()->GetResource<hh::fnd::ResReflection<app::rfl::AmyParameters>>("amy_common");
	//	auto* tailsRfl = hh::fnd::ResourceManager::GetInstance()->GetResource<hh::fnd::ResReflection<app::rfl::TailsParameters>>("tails_common");
	//	auto* knucklesRfl = hh::fnd::ResourceManager::GetInstance()->GetResource<hh::fnd::ResReflection<app::rfl::KnucklesParameters>>("knuckles_common");
	//	ReflectionSerializer::SerializeToFile(L"C:\\Users\\Ruben Tytgat\\source\\playercommon_extra_cyber\\base_sonic_cyberspace\\player_common_extra_cyber\\amy_modepackage_cyber.rfl", &sonicRfl->reflectionData->cyberspace);
	//	ReflectionSerializer::SerializeToFile(L"C:\\Users\\Ruben Tytgat\\source\\playercommon_extra_cyber\\base_sonic_cyberspace\\player_common_extra_cyber\\tails_modepackage_cyber.rfl", &sonicRfl->reflectionData->cyberspace);
	//	ReflectionSerializer::SerializeToFile(L"C:\\Users\\Ruben Tytgat\\source\\playercommon_extra_cyber\\base_sonic_cyberspace\\player_common_extra_cyber\\knuckles_modepackage_cyber.rfl", &sonicRfl->reflectionData->cyberspace);
	//	ReflectionSerializer::SerializeToFile(L"C:\\Users\\Ruben Tytgat\\source\\playercommon_extra_cyber\\base_cyberspace_sv\\player_common_extra_cyber\\amy_modepackage_cyber.rfl", &amyRfl->reflectionData->cyberspaceSV);
	//	ReflectionSerializer::SerializeToFile(L"C:\\Users\\Ruben Tytgat\\source\\playercommon_extra_cyber\\base_cyberspace_sv\\player_common_extra_cyber\\tails_modepackage_cyber.rfl", &tailsRfl->reflectionData->cyberspaceSV);
	//	ReflectionSerializer::SerializeToFile(L"C:\\Users\\Ruben Tytgat\\source\\playercommon_extra_cyber\\base_cyberspace_sv\\player_common_extra_cyber\\knuckles_modepackage_cyber.rfl", &knucklesRfl->reflectionData->cyberspaceSV);
	//	ReflectionSerializer::SerializeToFile(L"C:\\Users\\Ruben Tytgat\\source\\playercommon_extra_cyber\\base_forward_view\\player_common_extra_cyber\\amy_modepackage_cyber.rfl", &amyRfl->reflectionData->forwardView);
	//	ReflectionSerializer::SerializeToFile(L"C:\\Users\\Ruben Tytgat\\source\\playercommon_extra_cyber\\base_forward_view\\player_common_extra_cyber\\tails_modepackage_cyber.rfl", &tailsRfl->reflectionData->forwardView);
	//	ReflectionSerializer::SerializeToFile(L"C:\\Users\\Ruben Tytgat\\source\\playercommon_extra_cyber\\base_forward_view\\player_common_extra_cyber\\knuckles_modepackage_cyber.rfl", &knucklesRfl->reflectionData->forwardView);
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

void Desktop::HandleMousePicking()
{
	auto& io = ImGui::GetIO();

	pickerClicked = false;

	if (!io.WantCaptureMouse && !ImGui::IsAnyItemHovered()) {
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			auto* gameManager = GameManager::GetInstance();
			if (auto* cameraSrv = gameManager->GetService<hh::game::CameraManager>())
			if (auto* camera = cameraSrv->GetTopComponent(0)) {
				auto mouseStart = ImGui::GetMousePos() - ImGui::GetMouseDragDelta();
				auto mouseEnd = ImGui::GetMousePos();
				auto frustum = ScreenRectToFrustum(mouseStart, mouseEnd, (camera->viewportData.projMatrix * camera->viewportData.viewMatrix).inverse());

				if (ImGui::Begin("Overlay")) {
					ImGui::GetWindowDrawList()->AddRectFilled(mouseStart, mouseEnd, 0x40FFFFFF);
				}
				ImGui::End();

				pickedObjects.clear();
				for (auto* object : gameManager->objects)
					if (auto* gocTransform = object->GetComponent<GOCTransform>())
						if (frustum.Test(gocTransform->GetFrame().fullTransform.position))
							pickedObjects.push_back(object);

				locationPicked = false;
				pickerClicked = true;
			}
		}
		else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			auto* gameManager = GameManager::GetInstance();

			if (auto* physicsWorld = gameManager->GetService<hh::physics::PhysicsWorld>()) {
				if (auto* cameraSrv = gameManager->GetService<hh::game::CameraManager>())
				if (auto* camera = cameraSrv->GetTopComponent(0)) {
					auto ray = ScreenPosToWorldRay(ImGui::GetMousePos(), camera->viewportData.GetInverseViewMatrix() * camera->viewportData.projMatrix.inverse());

					csl::ut::MoveArray<PhysicsQueryResult> results{ hh::fnd::MemoryRouter::GetTempAllocator() };
					if (physicsWorld->RayCastAllHits(ray.start, ray.end, 0xFFFFFFFF, results)) {
						if (ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt)) {
							csl::ut::PointerMap<GameObject*, bool> unique{ hh::fnd::MemoryRouter::GetTempAllocator() };

							pickerResults.clear();

							for (auto& result : results) {
								if (auto* collider = hh::game::GameObjectSystem::GetComponentByHandle(result.collider)) {
									auto* gameObject = collider->GetOwnerGameObject();

									if (unique.Find(gameObject) == unique.end())
										pickerResults.push_back(result);

									unique.Insert(gameObject, true);
								}
							}

							ImGui::OpenPopup("Picker results");
							return;
						}
						else {
							for (auto& result : results) {
								if (auto* collider = hh::game::GameObjectSystem::GetComponentByHandle(result.collider)) {
									auto* gameObject = collider->GetOwnerGameObject();

									if (selectionColliderFilters[gameObject->layer][collider->filterCategory]) {
										pickedObjects.clear();
										pickedObjects.push_back(gameObject);
										pickedLocation = result.hitLocation;
										locationPicked = true;
										pickerClicked = true;
										return;
									}
								}
							}
							pickedObjects.clear();
							pickedLocation = results[0].hitLocation;
							locationPicked = true;
							pickerClicked = true;
						}
					}
					else {
						pickedObjects.clear();
						locationPicked = false;
						pickerClicked = true;
					}
				}
			}
		}
	}

	if (ImGui::BeginPopup("Picker results")) {
		for (auto& result : pickerResults) {
			if (auto* collider = hh::game::GameObjectSystem::GetComponentByHandle(result.collider)) {
				auto* gameObject = collider->GetOwnerGameObject();

				if (ImGui::Selectable(gameObject->name)) {
					pickedObjects.clear();
					pickedObjects.push_back(collider->GetOwnerGameObject());
					locationPicked = false;
					pickerClicked = true;
				}
			}
		}
		ImGui::EndPopup();
	}
}

Desktop::~Desktop()
{
	delete operationMode;
}

bool Desktop::IsPickerMouseReleased() const
{
	return pickerClicked;
}

const csl::ut::MoveArray<GameObject*>& Desktop::GetPickedObjects() const {
	return pickedObjects;
}

const csl::math::Vector3* Desktop::GetPickedLocation() const {
	return locationPicked ? &pickedLocation : nullptr;
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

void Desktop::SwitchToObjectInspectionMode()
{
	if (operationMode != nullptr)
		operationMode->DeinitBehaviors();

	operationMode = new (GetAllocator()) ObjectInspection(GetAllocator());

	operationMode->InitBehaviors();
}

void Desktop::SwitchToLevelEditorMode()
{
	if (operationMode != nullptr)
		operationMode->DeinitBehaviors();

	operationMode = new (GetAllocator()) LevelEditor(GetAllocator());

	operationMode->InitBehaviors();
}

#ifdef DEVTOOLS_TARGET_SDK_rangers
void Desktop::SwitchToSurfRideEditorMode()
{
	if (operationMode != nullptr)
		operationMode->DeinitBehaviors();

	operationMode = new (GetAllocator()) SurfRideEditor(GetAllocator());

	operationMode->InitBehaviors();
}
#endif
