#include "common/Icons.h"
#include "Desktop.h"
#include "ToolBar.h"
#include "SettingsManager.h"
#include "resources/ResourceBrowser.h"
#include "game-services/GameServiceInspector.h"
#include "operation-modes/ObjectInspection/ObjectInspection.h"
#include "operation-modes/LevelEditor/LevelEditor.h"

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

	//gameViewerCtx = viewerManagerEmulator.viewerContextManager->CreateViewerContext<GameViewerContext>();
	//gameViewerCtx->gameManagers.push_back(GameManager::GetInstance());
	//objViewerCtx = viewerManagerEmulator.viewerContextManager->CreateViewerContext<ObjectViewerContext>();
	//physicsViewerCtx = viewerManagerEmulator.viewerContextManager->CreateViewerContext<PhysicsViewerContext>();

	//*rangerssdk::GetAddress(&hh::dbg::ViewerManager::instance) = &viewerManagerEmulator;

	//picker = viewerManagerEmulator.CreateViewer<MousePickingViewer>();
	//physicsPicker = viewerManagerEmulator.CreateViewer<PhysicsMousePickingViewer>();
	//physicsPicker->unk1 = true;

	SwitchToObjectInspectionMode();
}

void Desktop::Render() {
	HandleMousePicking();

	ToolBar::Render();
	operationMode->Render();
	ResourceBrowser::RenderDialogs();

	csl::ut::MoveArray<StandaloneWindow*> windowsThatWantToClose{ hh::fnd::MemoryRouter::GetTempAllocator() };

	for (auto& window : windows) {
		if (!window->Render())
			windowsThatWantToClose.push_back(window);
	}

	for (auto* window : windowsThatWantToClose) {
		RemoveStandaloneWindow(window);
	}

	SettingsManager::Render();

	//prevPhysicsPickerMouseDown = physicsPicker->mouseDown;
}

void Desktop::AddStandaloneWindow(StandaloneWindow* window) {
	this->windows.push_back(hh::fnd::Reference<StandaloneWindow>{ window });
}

void Desktop::RemoveStandaloneWindow(StandaloneWindow* window) {
	this->windows.remove(this->windows.find(window));
}

void Desktop::HandleMousePicking()
{
	auto& io = ImGui::GetIO();

	pickerClicked = false;

	if (!io.WantCaptureMouse && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
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
									pickedObject = gameObject;
									pickedLocation = result.hitLocation;
									locationPicked = true;
									pickerClicked = true;
									return;
								}
							}
						}
						pickedObject = nullptr;
						pickedLocation = results[0].hitLocation;
						locationPicked = true;
						pickerClicked = true;
					}
				}
				else {
					pickedObject = nullptr;
					locationPicked = false;
					pickerClicked = true;
				}
			}
		}
	}
	//}


	if (ImGui::BeginPopup("Picker results")) {
		for (auto& result : pickerResults) {
			if (auto* collider = result.collider.Get(*(rangerssdk::GetAddress(&hh::game::GameObjectSystem::handleManager)))) {
				auto* gameObject = collider->GetOwnerGameObject();

				if (ImGui::Selectable(gameObject->pObjectName)) {
					pickedObject = collider->GetOwnerGameObject();
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

GameObject* Desktop::GetPickedObject() const {
	return pickedObject == nullptr ? nullptr : pickedObject;
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
