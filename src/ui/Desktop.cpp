#include "common/Icons.h"
#include "Desktop.h"
#include "ToolBar.h"
#include "SettingsManager.h"
//#include "resources/ResourceBrowser.h"
//#include "game-services/GameServiceInspector.h"
#include "operation-modes/ObjectInspection/ObjectInspection.h"
#include "operation-modes/LevelEditor/LevelEditor.h"
//#include "operation-modes/SurfRideEditor/SurfRideEditor.h"
#include "reflection/serialization/ReflectionSerializer.h"
#include <utilities/math/MathUtils.h>
#include <utilities/CompatibleObject.h>
#include "common/editors/Basic.h"

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

	SwitchToObjectInspectionMode();
}

namespace app::game {
	class GrindService : hh::game::GameService {
		GAMESERVICE_CLASS_DECLARATION(GrindService)
	};
}
void Desktop::Render() {
	RenderOverlayWindow();
	HandleMousePicking();

	ToolBar::Render();
	operationMode->Render();
	//ResourceBrowser::RenderDialogs();

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
			if (auto* camera = gameManager->GetService<hh::game::CameraManager>()->GetTopComponent(0)) {
				auto mouseStart = ImGui::GetMousePos() - ImGui::GetMouseDragDelta();
				auto mouseEnd = ImGui::GetMousePos();
				auto frustum = ScreenRectToFrustum(mouseStart, mouseEnd, (camera->viewportData.projMatrix * camera->viewportData.viewMatrix).inverse());

				ImGui::Begin("Overlay");
				ImGui::GetWindowDrawList()->AddRectFilled(mouseStart, mouseEnd, 0x40FFFFFF);
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
			ImVec2 ndcPos{ 2 * io.MousePos.x / io.DisplaySize.x - 1, 1 - 2 * io.MousePos.y / io.DisplaySize.y };

			auto* gameManager = GameManager::GetInstance();

			if (auto* physicsWorld = gameManager->GetService<hh::physics::PhysicsWorld>()) {
				if (auto* camera = gameManager->GetService<hh::game::CameraManager>()->GetTopComponent(0)) {
					auto inverseMat = camera->viewportData.GetInverseViewMatrix() * camera->viewportData.projMatrix.inverse();

					Eigen::Vector4f worldSpaceOrigin = inverseMat * csl::math::Vector4{ ndcPos.x, ndcPos.y, 0, 1 };
					Eigen::Vector4f worldSpaceTarget = inverseMat * csl::math::Vector4{ ndcPos.x, ndcPos.y, 1, 1 };

					csl::ut::MoveArray<PhysicsQueryResult> results{ hh::fnd::MemoryRouter::GetTempAllocator() };
					if (physicsWorld->RayCastAllHits({ worldSpaceOrigin.hnormalized() }, { worldSpaceTarget.hnormalized() }, 0xFFFFFFFF, results)) {
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

//bool Desktop::IsPickerMouseDown() const
//{
//	return physicsPicker->mouseDown;
//}

bool Desktop::IsPickerMouseReleased() const
{
	return pickerClicked;
}

//bool Desktop::IsPickerMouseClicked() const
//{
//	return !prevPhysicsPickerMouseDown && physicsPicker->mouseDown;
//}

const csl::ut::MoveArray<GameObject*>& Desktop::GetPickedObjects() const {
	return pickedObjects;
}

const csl::math::Vector3* Desktop::GetPickedLocation() const {
	return locationPicked ? &pickedLocation : nullptr;
}

void Desktop::SwitchToObjectInspectionMode()
{
	operationMode = new (GetAllocator()) ObjectInspection(GetAllocator());
}

void Desktop::SwitchToLevelEditorMode()
{
	operationMode = new (GetAllocator()) LevelEditor(GetAllocator());
}
//
//void Desktop::SwitchToSurfRideEditorMode()
//{
//	operationMode = new (GetAllocator()) SurfRideEditor(GetAllocator());
//}
