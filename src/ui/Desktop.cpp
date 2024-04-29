#include "common/Icons.h"
#include "Desktop.h"
#include "ToolBar.h"
#include "SettingsManager.h"
#include "resources/ResourceBrowser.h"
#include "game-services/GameServiceInspector.h"
#include "operation-modes/ObjectInspection/ObjectInspection.h"
#include "operation-modes/LevelEditor/LevelEditor.h"
#include "reflection/serialization/ReflectionSerializer.h"

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::physics;

Desktop* Desktop::instance{};
bool Desktop::selectionColliderFilters[32][32]{ true };

Desktop::Desktop(csl::fnd::IAllocator* allocator) : BaseObject{ allocator }
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
	HandleMousePicking();

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

	if (auto* s = GameManager::GetInstance()->GetService<app::game::GrindService>()) {
		ImGui::Text("%d", *reinterpret_cast<bool*>(reinterpret_cast<size_t>(s) + 0x1d0));
		ImGui::Checkbox("grind", reinterpret_cast<bool*>(reinterpret_cast<size_t>(s) + 0x1d0));
	}

	if (ImGui::Button("Add island objinfo")) {

		auto* moduleAllocator = hh::fnd::MemoryRouter::GetModuleAllocator();
		auto* islandObjInfo = new (moduleAllocator) app::IslandObjInfo(moduleAllocator);
		GameManager::GetInstance()->GetService<ObjInfoContainer>()->Register(islandObjInfo->GetInfoName(), islandObjInfo);

	}

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
				auto cameraMatrix{ camera->viewportData.projMatrix * camera->viewportData.viewMatrix };
				auto inverseCameraMatrix{ cameraMatrix.inverse() };
				auto* ivp = ImGui::GetMainViewport();
				auto mouseStart = ImGui::GetMousePos() - ImGui::GetMouseDragDelta();
				auto mouseEnd = ImGui::GetMousePos();
				auto topLeft = ImVec2{ std::fminf(mouseStart.x, mouseEnd.x), std::fminf(mouseStart.y, mouseEnd.y) };
				auto botRight = ImVec2{ std::fmaxf(mouseStart.x, mouseEnd.x), std::fmaxf(mouseStart.y, mouseEnd.y) };

				ImGui::SetNextWindowSize(ivp->Size);
				ImGui::SetNextWindowPos(ivp->Pos);

				ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
				ImGui::PushStyleColor(ImGuiCol_Border, 0);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

				ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

				auto* drawList = ImGui::GetWindowDrawList();
				drawList->AddRectFilled(ImGui::GetMousePos() - ImGui::GetMouseDragDelta(), ImGui::GetMousePos(), 0x40FFFFFF);

				ImGui::End();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor(3);

				ImVec2 ndcPosTopLeft{ 2 * topLeft.x / io.DisplaySize.x - 1, 1 - 2 * topLeft.y / io.DisplaySize.y };
				ImVec2 ndcPosBotRight{ 2 * botRight.x / io.DisplaySize.x - 1, 1 - 2 * botRight.y / io.DisplaySize.y };

				Eigen::Vector3f nearTopLeft{ (inverseCameraMatrix * Eigen::Vector4f{ ndcPosTopLeft.x, ndcPosTopLeft.y, 0, 1 }).hnormalized() };
				Eigen::Vector3f nearTopRight{ (inverseCameraMatrix * Eigen::Vector4f{ ndcPosBotRight.x, ndcPosTopLeft.y, 0, 1 }).hnormalized() };
				Eigen::Vector3f nearBotRight{ (inverseCameraMatrix * Eigen::Vector4f{ ndcPosBotRight.x, ndcPosBotRight.y, 0, 1 }).hnormalized() };
				Eigen::Vector3f nearBotLeft{ (inverseCameraMatrix * Eigen::Vector4f{ ndcPosTopLeft.x, ndcPosBotRight.y, 0, 1 }).hnormalized() };
				Eigen::Vector3f farTopLeft{ (inverseCameraMatrix * Eigen::Vector4f{ ndcPosTopLeft.x, ndcPosTopLeft.y, 1, 1 }).hnormalized() };
				Eigen::Vector3f farTopRight{ (inverseCameraMatrix * Eigen::Vector4f{ ndcPosBotRight.x, ndcPosTopLeft.y, 1, 1 }).hnormalized() };
				Eigen::Vector3f farBotRight{ (inverseCameraMatrix * Eigen::Vector4f{ ndcPosBotRight.x, ndcPosBotRight.y, 1, 1 }).hnormalized() };
				Eigen::Vector3f farBotLeft{ (inverseCameraMatrix * Eigen::Vector4f{ ndcPosTopLeft.x, ndcPosBotRight.y, 1, 1 }).hnormalized() };

				Eigen::Hyperplane<float, 3> frustum[]{
					{ (farBotLeft - nearBotLeft).cross(nearTopLeft - nearBotLeft), nearBotLeft },
					{ (farTopLeft - nearTopLeft).cross(nearTopRight - nearTopLeft), nearTopLeft },
					{ (farTopRight - nearTopRight).cross(nearBotRight - nearTopRight), nearTopRight },
					{ (farBotRight - nearBotRight).cross(nearBotLeft - nearBotRight), nearBotRight },
					{ (nearBotLeft - nearBotRight).cross(nearTopRight - nearBotRight), nearBotRight },
					{ (farBotRight - farBotLeft).cross(farTopLeft - farBotLeft), farBotLeft },
				};

				pickedObjects.clear();
				for (auto* object : gameManager->objects) {
					if (auto* gocTransform = object->GetComponent<GOCTransform>()) {
						bool foundCounter{ false };

						for (size_t i = 0; i < 6; i++) {
							if (frustum[i].signedDistance(gocTransform->frame->fullTransform.position) < 0) {
								foundCounter = true;
								break;
							}
						}

						if (!foundCounter)
							pickedObjects.push_back(object);
					}
				}
				locationPicked = false;
				pickerClicked = true;
			}
		}
		else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			ImVec2 ndcPos{ 2 * io.MousePos.x / io.DisplaySize.x - 1, 1 - 2 * io.MousePos.y / io.DisplaySize.y };

			auto* gameManager = GameManager::GetInstance();

			if (auto* physicsWorld = gameManager->GetService<hh::physics::PhysicsWorldBullet>()) {
				if (auto* camera = gameManager->GetService<hh::game::CameraManager>()->GetTopComponent(0)) {
					auto inverseMat = camera->viewportData.inverseViewMatrix * camera->viewportData.projMatrix.inverse();

					Eigen::Vector4f worldSpaceOrigin = inverseMat * csl::math::Vector4{ ndcPos.x, ndcPos.y, 0, 1 };
					Eigen::Vector4f worldSpaceTarget = inverseMat * csl::math::Vector4{ ndcPos.x, ndcPos.y, 1, 1 };

					//PhysicsQueryResult result{};
					//if (physicsWorld->RayCastClosest({ worldSpaceOrigin.hnormalized() }, { worldSpaceTarget.hnormalized() }, 0xFDFFFFFF, result)) {
					//	auto* collider = result.collider.Get(*(rangerssdk::GetAddress(&hh::game::GameObjectSystem::handleManager)));

					//	pickedObject = collider ? collider->GetOwnerGameObject() : nullptr;
					//	pickedLocation = result.hitLocation;
					//}
					//else {
					//	pickedObject = nullptr;
					//	pickedLocation = csl::math::Vector3{};
					//}

					csl::ut::MoveArray<PhysicsQueryResult> results{ hh::fnd::MemoryRouter::GetTempAllocator() };
					if (physicsWorld->RayCastAllHits({ worldSpaceOrigin.hnormalized() }, { worldSpaceTarget.hnormalized() }, 0xFFFFFFFF, results)) {
						if (ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt)) {
							csl::ut::PointerMap<GameObject*, bool> unique{ hh::fnd::MemoryRouter::GetTempAllocator() };

							pickerResults.clear();

							for (auto& result : results) {
								if (auto* collider = result.collider.Get(*(rangerssdk::GetAddress(&hh::game::GameObjectSystem::handleManager)))) {
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
								if (auto* collider = result.collider.Get(*(rangerssdk::GetAddress(&hh::game::GameObjectSystem::handleManager)))) {
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
	//}


	if (ImGui::BeginPopup("Picker results")) {
		for (auto& result : pickerResults) {
			if (auto* collider = result.collider.Get(*(rangerssdk::GetAddress(&hh::game::GameObjectSystem::handleManager)))) {
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
