#include "Pch.h"
#include "Desktop.h"
#include "ToolBar.h"
#include "common/Icons.h"
#include "SettingsManager.h"
#include "ResourceBrowser.h"
#include "operation-modes/ObjectInspection/ObjectInspection.h"
#include "operation-modes/LevelEditor/LevelEditor.h"
#include "GameServiceInspector.h"

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::physics;

Desktop* Desktop::instance{};

Desktop::Desktop(csl::fnd::IAllocator* allocator) : BaseObject{ allocator }
{
	resourceLoader = hh::fnd::ResourceLoader::Create(allocator);
	resourceLoader->LoadPackfile("mods/angryzor_devtools/devtools.pac", 0);

	Translations::Init(allocator);

	gameViewerCtx = viewerManagerEmulator.viewerContextManager->CreateViewerContext<GameViewerContext>();
	gameViewerCtx->gameManagers.push_back(GameManager::GetInstance());
	objViewerCtx = viewerManagerEmulator.viewerContextManager->CreateViewerContext<ObjectViewerContext>();
	physicsViewerCtx = viewerManagerEmulator.viewerContextManager->CreateViewerContext<PhysicsViewerContext>();

	*rangerssdk::GetAddress(&hh::dbg::ViewerManager::instance) = &viewerManagerEmulator;

	//picker = viewerManagerEmulator.CreateViewer<MousePickingViewer>();
	physicsPicker = viewerManagerEmulator.CreateViewer<PhysicsMousePickingViewer>();
	physicsPicker->unk1 = true;

	SwitchToObjectInspectionMode();
}

void Desktop::Render() {
	ToolBar::Render();
	operationMode->Render();
	GameServiceInspector::Render();
	ResourceBrowser::RenderDialogs();

	auto* ahm = app::fnd::AppHeapManager::GetInstance();

	ImGui::Text("Resident");
	ImGui::Text("Live allocations: %d", ahm->residentAllocator.heap.liveAllocations);
	ImGui::Text("Total allocations: %d", ahm->residentAllocator.heap.totalAllocations);
	ImGui::Text("Block count: %d", ahm->residentAllocator.heap.blockCount);

	ImGui::Text("Cyloop");
	ImGui::Text("Live allocations: %d", ahm->cyloopAllocator.heap.liveAllocations);
	ImGui::Text("Total allocations: %d", ahm->cyloopAllocator.heap.totalAllocations);
	ImGui::Text("Block count: %d", ahm->cyloopAllocator.heap.blockCount);

	ImGui::Text("Heightfield");
	ImGui::Text("Live allocations: %d", ahm->heightfieldAllocator.heap.liveAllocations);
	ImGui::Text("Total allocations: %d", ahm->heightfieldAllocator.heap.totalAllocations);
	ImGui::Text("Block count: %d", ahm->heightfieldAllocator.heap.blockCount);

	ImGui::Text("Needle");
	ImGui::Text("Live allocations: %d", ahm->needleAllocator->heap.liveAllocations);
	ImGui::Text("Total allocations: %d", ahm->needleAllocator->heap.totalAllocations);
	ImGui::Text("Block count: %d", ahm->needleAllocator->heap.blockCount);

	csl::ut::MoveArray<StandaloneWindow*> windowsThatWantToClose{ hh::fnd::MemoryRouter::GetTempAllocator() };

	for (auto& window : windows) {
		if (!window->Render())
			windowsThatWantToClose.push_back(window);
	}

	for (auto* window : windowsThatWantToClose) {
		RemoveStandaloneWindow(window);
	}

	SettingsManager::Render();

	prevPhysicsPickerMouseDown = physicsPicker->mouseDown;
}

void Desktop::AddStandaloneWindow(StandaloneWindow* window) {
	this->windows.push_back(hh::fnd::Reference<StandaloneWindow>{ window });
}

void Desktop::RemoveStandaloneWindow(StandaloneWindow* window) {
	this->windows.remove(this->windows.find(window));
}

Desktop::~Desktop()
{
	delete operationMode;
}

bool Desktop::IsPickerMouseDown() const
{
	return physicsPicker->mouseDown;
}

bool Desktop::IsPickerMouseReleased() const
{
	return prevPhysicsPickerMouseDown && !physicsPicker->mouseDown;
}

bool Desktop::IsPickerMouseClicked() const
{
	return !prevPhysicsPickerMouseDown && physicsPicker->mouseDown;
}

GameObject* Desktop::GetPickedObject() const {
	return physicsViewerCtx->pickedObject.collider == nullptr ? nullptr : physicsViewerCtx->pickedObject.collider->GetOwnerGameObject();
}

csl::math::Vector3* Desktop::GetPickedLocation() const {
	return physicsViewerCtx->pickedObject.collider == nullptr ? nullptr : &physicsViewerCtx->pickedObject.unk2; //&physicsPicker->mouseSpringAction.unk1;
}

void Desktop::SwitchToObjectInspectionMode()
{
	operationMode = new (GetAllocator()) ObjectInspection(GetAllocator());
}

void Desktop::SwitchToLevelEditorMode()
{
	operationMode = new (GetAllocator()) LevelEditor(GetAllocator());
}
