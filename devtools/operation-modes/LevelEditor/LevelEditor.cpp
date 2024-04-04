#include "../../Pch.h"
#include "LevelEditor.h"
#include "../../GameServiceInspector.h"
#include "../../ResourceBrowser.h"
#include "../../common/SimpleWidgets.h"
#include "../../Desktop.h"
#include "../../ObjectDataUtils.h"

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::physics;

LevelEditor::LevelEditor(csl::fnd::IAllocator* allocator) : OperationMode{ allocator }, mt{ std::random_device()() }
{
	auto* gameManager = GameManager::GetInstance();

	gameManager->AddListener(this);

	if (auto* objWorld = gameManager->GetService<ObjectWorld>())
		objWorld->AddWorldListener(this);
}

LevelEditor::~LevelEditor() {
	auto* gameManager = GameManager::GetInstance();

	gameManager->RemoveListener(this);

	if (auto* objWorld = gameManager->GetService<ObjectWorld>())
		objWorld->RemoveWorldListener(this);
}

void LevelEditor::Render() {
	setObjectList.Render();
	objectDataInspector.Render();
	objectLibrary.Render();

	if (!focusedChunk)
		return;

	// Object manipulation
	if (focusedObject)
		HandleObjectManipulation();

	HandleObjectSelection();
}

void LevelEditor::HandleObjectSelection() {
	if (Desktop::instance->IsPickerMouseReleased()) {
		if (auto* picked = Desktop::instance->GetPickedObject()) {
			auto* status = picked->GetWorldObjectStatus();

			focusedObject = status == nullptr || focusedChunk->GetObjectIndexById(status->objectData->id) == -1 ? nullptr : status->objectData;
		}
		else
			focusedObject = nullptr;
	}

	if (Desktop::instance->IsPickerMouseReleased() && objectClassToPlace && placeTargetLayer && Desktop::instance->GetPickedLocation()) {
		SpawnObject();
		setObjectList.InvalidateTree();
	}

	CheckSelectionHotkeys();
}

void LevelEditor::HandleObjectManipulation() {
	auto* gameManager = GameManager::GetInstance();

	if (auto* camera = gameManager->GetService<hh::game::CameraManager>()->GetTopComponent(0)) {
		auto* gctx = hh::gfnd::GraphicsContext::GetInstance();

		ImGuiIO& io = ImGui::GetIO();

		// We are operating the gizmo on the absolute transform so ImGuizmo can position it correctly.
		auto absoluteTransform = ObjectTransformDataToAffine3f(focusedObject->transform);

		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(camera->viewportData.viewMatrix.data(), camera->viewportData.projMatrix.data(), gizmoOperation, gizmoMode, absoluteTransform.data(), NULL, NULL);

		// Update both transforms based on this changed absolute transform.
		UpdateAbsoluteTransform(absoluteTransform, *focusedObject);

		// Copy changes to the live object if any.
		int idx = focusedChunk->GetObjectIndexById(focusedObject->id);
		if (idx != -1)
			if (auto* obj = focusedChunk->GetObjectByIndex(idx))
				if (auto* gocTransform = obj->GetComponent<GOCTransform>())
					UpdateGOCTransform(*focusedObject, *gocTransform);

		CheckGizmoHotkeys();
	}
}

void LevelEditor::CheckGizmoHotkeys() {
	if ((ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt)) && ImGui::IsKeyPressed(ImGuiKey_Space))
		gizmoMode = gizmoMode == ImGuizmo::LOCAL ? ImGuizmo::WORLD : ImGuizmo::LOCAL;

	if (ImGui::IsKeyPressed(ImGuiKey_G))
		gizmoOperation = ImGuizmo::TRANSLATE;

	if (ImGui::IsKeyPressed(ImGuiKey_R))
		gizmoOperation = ImGuizmo::ROTATE;

	if (gizmoOperation & ImGuizmo::TRANSLATE) {
		if (ImGui::IsKeyPressed(ImGuiKey_X))
			gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::TRANSLATE_Y | ImGuizmo::TRANSLATE_Z : ImGuizmo::TRANSLATE_X;

		if (ImGui::IsKeyPressed(ImGuiKey_Y))
			gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Z : ImGuizmo::TRANSLATE_Y;

		if (ImGui::IsKeyPressed(ImGuiKey_Z))
			gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Y : ImGuizmo::TRANSLATE_Z;
	}

	if (gizmoOperation & ImGuizmo::ROTATE) {
		if (ImGui::IsKeyPressed(ImGuiKey_X))
			gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::ROTATE_Y | ImGuizmo::ROTATE_Z : ImGuizmo::ROTATE_X;

		if (ImGui::IsKeyPressed(ImGuiKey_Y))
			gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Z : ImGuizmo::ROTATE_Y;

		if (ImGui::IsKeyPressed(ImGuiKey_Z))
			gizmoOperation = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift) ? ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Y : ImGuizmo::ROTATE_Z;
	}
}

void LevelEditor::CheckSelectionHotkeys() {
	if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
		DeleteFocusedObject();
		setObjectList.InvalidateTree();
	}

	if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
		focusedObject = nullptr;
	}
}

void LevelEditor::DeleteFocusedObject() {
	focusedChunk->Despawn(focusedObject);

	for (auto* layer : focusedChunk->GetLayers())
		for (auto* object : layer->GetResource()->GetObjects())
			if (object == focusedObject) {
				layer->RemoveObjectData(focusedObject);
				focusedObject = nullptr;
				return;
			}
}

struct RangeSpawningData {
	float range;
	float distance;
};

void LevelEditor::SpawnObject() {
	auto* resource = placeTargetLayer->GetResource();
	auto* alloc = resource->GetAllocator();

	char name[200];
	snprintf(name, sizeof(name), "%s_%zd", objectClassToPlace->pName, resource->GetObjects().size());

	auto* objData = new (alloc) ObjectData{
		alloc,
		objectClassToPlace,
		{ mt(), mt() },
		name,
		focusedObject,
		{ csl::math::Position{ *Desktop::instance->GetPickedLocation() }, csl::math::Position{ 0, 0, 0 } }
	};

	if (auto* objInfoAttribute = objectClassToPlace->GetAttribute("objinfo")) {
		auto* objInfoContainer = GameManager::GetInstance()->GetService<ObjInfoContainer>();
		auto* objInfoName = static_cast<const char*>(objInfoAttribute->m_Value);

		if (objInfoContainer->GetInfo(objInfoName) == nullptr) {
			auto* objInfoClass = (*rangerssdk::GetAddress(&ObjInfoRegistry::instance))->objInfosByName.GetValueOrFallback(objInfoName, nullptr);
			auto* objInfo = objInfoClass->Create(GetAllocator());

			objInfoContainer->Register(objInfo->GetInfoName(), objInfo);
		}
	}

	auto* rangeSpawningData = new (GetAllocator()) RangeSpawningData{ 50, 20 };
	auto* rangeSpawning = new (GetAllocator()) ComponentData{ GetAllocator(), GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName("RangeSpawning"), rangeSpawningData };
	objData->componentData.push_back(rangeSpawning);

	resource->AddObject(objData);
	focusedChunk->AddWorldObjectStatus(objData, true, 0);
	focusedObject = objData;
}

void LevelEditor::GameServiceAddedCallback(GameService* service) {
	if (service->pStaticClass == ObjectWorld::GetClass()) {
		auto* objWorld = static_cast<ObjectWorld*>(service);
		objWorld->AddWorldListener(this);
	}
}

void LevelEditor::GameServiceRemovedCallback(GameService* service) {
	if (service->pStaticClass == ObjectWorld::GetClass()) {
		auto* objWorld = static_cast<ObjectWorld*>(service);
		objWorld->RemoveWorldListener(this);
	}
}

void LevelEditor::WorldChunkRemovedCallback(ObjectWorldChunk* chunk)
{
	if (focusedChunk == chunk)
		SetFocusedChunk(nullptr);
}

void LevelEditor::SetFocusedChunk(ObjectWorldChunk* chunk)
{
	if (focusedChunk == chunk)
		return;

	placeTargetLayer = nullptr;
	objectClassToPlace = nullptr;
	focusedObject = nullptr;
	if (focusedChunk) {
		focusedChunk->DespawnAll();
		focusedChunk->ShutdownPendingObjects();
		focusedChunk->SetEditorStatus(false);
	}
	focusedChunk = chunk;
	if (focusedChunk) {
		focusedChunk->SetEditorStatus(true);
		focusedChunk->DespawnAll();
		focusedChunk->ShutdownPendingObjects();
		focusedChunk->Restart(true);
	}
	setObjectList.InvalidateTree();
}
