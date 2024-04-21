#include "LevelEditor.h"
#include <ui/common/SimpleWidgets.h>
#include <ui/Desktop.h>
#include <utilities/ObjectDataUtils.h>
#include <utilities/BoundingBoxes.h>
#include <utilities/math/MathUtils.h>

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

void LevelEditor::RenderDebugVisuals(hh::gfnd::DrawContext& ctx)
{
	if (haveSelectionAabb) {
		csl::geom::Aabb aabb{ selectionAabb.m_Min, selectionAabb.m_Max };
		ctx.DrawAABB(aabb.m_Min, aabb.m_Max, { 140, 255, 255, 255 });
	}
}

void LevelEditor::Render() {
	setObjectList.Render();
	objectDataInspector.Render();
	objectLibrary.Render();

	if (ImGui::Begin("Sonic Frontiers Devtools")) {
		ImGui::SameLine();
		ImGui::Checkbox("Render debug comments", &renderDebugComments);
	}
	ImGui::End();
	
	if (renderDebugComments)
		RenderDebugComments();

	if (!focusedChunk)
		return;

	// Object manipulation
	if (focusedObjects.size() > 0) {
		csl::ut::MoveArray<GameObject*> gameObjects{ hh::fnd::MemoryRouter::GetTempAllocator() };
		for (auto* obj : focusedObjects)
			if (auto* gameObject = focusedChunk->GetGameObjectByObjectId(obj->id))
				gameObjects.push_back(gameObject);

		haveSelectionAabb = CalcApproxAabb(gameObjects, selectionAabb);
		HandleObjectManipulation();
	}
	else {
		haveSelectionAabb = false;
	}

	HandleObjectSelection();
}

void LevelEditor::RenderDebugComments()
{
	auto* gameManager = GameManager::GetInstance();

	if (auto* camera = gameManager->GetService<hh::game::CameraManager>()->GetTopComponent(0)) {
		auto cameraMatrix{ camera->viewportData.projMatrix * camera->viewportData.viewMatrix };
		auto* ivp = ImGui::GetMainViewport();

		ImGui::SetNextWindowSize(ivp->Size);
		ImGui::SetNextWindowPos(ivp->Pos);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
		ImGui::PushStyleColor(ImGuiCol_Border, 0);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		ImGui::Begin("Debug text", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

		for (auto* obj : gameManager->objects) {
			if (auto* gocTransform = obj->GetComponent<GOCTransform>()) {
				hh::dbg::MsgGetDebugCommentInEditor msgGetDbgCmt{};

				obj->ProcessMessage(msgGetDbgCmt);
				if (msgGetDbgCmt.comment[0] == 0)
					continue;

				Eigen::Matrix4f fullMatrix{ cameraMatrix * TransformToAffine3f(gocTransform->frame->fullTransform).matrix() };
				Eigen::Vector4f clipSpaceOrigin{ fullMatrix * Eigen::Vector4f{ 0.0f, 0.0f, 0.0f, 1.0f } };

				// Cull
				if (clipSpaceOrigin.x() < -clipSpaceOrigin.w() || clipSpaceOrigin.x() > clipSpaceOrigin.w()
					|| clipSpaceOrigin.y() < -clipSpaceOrigin.w() || clipSpaceOrigin.y() > clipSpaceOrigin.w()
					|| clipSpaceOrigin.z() <= 0 || clipSpaceOrigin.z() > clipSpaceOrigin.w())
					continue;

				Eigen::Vector4f transformedOrigin{ clipSpaceOrigin * 0.5 / clipSpaceOrigin.w() };

				auto originInImGuiSpace = ImVec2{ (transformedOrigin.x() + 0.5f) * ivp->Size.x, (0.5f - transformedOrigin.y()) * ivp->Size.y };

				auto textSize = ImGui::CalcTextSize(msgGetDbgCmt.comment);

				ImGui::SetCursorPos(originInImGuiSpace - textSize / 2);
				ImGui::Text(msgGetDbgCmt.comment);
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);
	}
}

void LevelEditor::HandleObjectSelection() {
	if (Desktop::instance->IsPickerMouseReleased()) {
		auto& picked = Desktop::instance->GetPickedObjects();
		focusedObjects.clear();
		for (auto* object : picked) {
			auto* status = object->GetWorldObjectStatus();

			if (status != nullptr && focusedChunk->GetObjectIndexById(status->objectData->id) != -1)
				focusedObjects.push_back(status->objectData);
		}
	}

	if (Desktop::instance->IsPickerMouseReleased() && objectClassToPlace && placeTargetLayer && Desktop::instance->GetPickedLocation())
		SpawnObject();

	CheckSelectionHotkeys();
}

void LevelEditor::HandleObjectManipulation() {
	auto* gameManager = GameManager::GetInstance();

	if (auto* camera = gameManager->GetService<hh::game::CameraManager>()->GetTopComponent(0)) {
		auto* gctx = hh::gfnd::GraphicsContext::GetInstance();

		ImGuiIO& io = ImGui::GetIO();

		Eigen::Affine3f pivotTransform{};
		if (focusedObjects.size() > 1 && haveSelectionAabb)
			pivotTransform.fromPositionOrientationScale((selectionAabb.m_Min + selectionAabb.m_Max) / 2.0f, Eigen::Quaternionf::Identity(), Eigen::Vector3f{ 1.0f, 1.0f, 1.0f });
		else
			pivotTransform = ObjectTransformDataToAffine3f(focusedObjects[0]->transform);

		csl::ut::MoveArray<ObjectData*> objsToMove{ hh::fnd::MemoryRouter::GetTempAllocator() };
		for (auto& object : focusedObjects) {
			ObjectData* obj = object;
			bool keepLooking{ true };
			while (keepLooking) {
				if (obj->parentID != ObjectId{ 0, 0 }) {
					for (auto& obj2 : focusedObjects) {
						if (obj2->id == obj->parentID) {
							keepLooking = false;
							break;
						}
					}

					obj = focusedChunk->GetWorldObjectStatusByObjectId(obj->parentID).objectData;
				}
				else {
					objsToMove.push_back(object);
					keepLooking = false;
				}
			}
		}

		csl::ut::MoveArray<Eigen::Affine3f> preTransforms{ hh::fnd::MemoryRouter::GetTempAllocator() };

		for (auto& object : objsToMove) {
			auto absoluteTransform = ObjectTransformDataToAffine3f(object->transform);
			auto localTransform = ObjectTransformDataToAffine3f(object->localTransform);
			preTransforms.push_back(pivotTransform.inverse() * absoluteTransform);
		}

		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

		if (ImGuizmo::Manipulate(camera->viewportData.viewMatrix.data(), camera->viewportData.projMatrix.data(), gizmoOperation, gizmoMode, pivotTransform.data(), NULL, NULL)) {
			size_t i{ 0 };
			for (auto& object : objsToMove) {
				// Update both transforms based on this changed absolute transform.
				UpdateAbsoluteTransform(pivotTransform * preTransforms[i], *object);
				i++;

				// Copy changes to the live object if any.
				int idx = focusedChunk->GetObjectIndexById(object->id);
				if (idx != -1)
					if (auto* obj = focusedChunk->GetObjectByIndex(idx))
						if (auto* gocTransform = obj->GetComponent<GOCTransform>())
							UpdateGOCTransform(*object, *gocTransform);
			}
		}

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
	if (ImGui::IsKeyPressed(ImGuiKey_Delete))
		DeleteFocusedObject();

	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
		Deselect();
}

void LevelEditor::DeleteFocusedObject() {
	for (auto& focusedObject : focusedObjects) {
		focusedChunk->Despawn(focusedObject);

		for (auto* layer : focusedChunk->GetLayers())
			for (auto* object : layer->GetResource()->GetObjects())
				if (object == focusedObject)
					layer->RemoveObjectData(focusedObject);
	}

	Deselect();

	setObjectList.InvalidateTree();
}

void LevelEditor::ReloadObjectWorldData()
{
	setObjectList.InvalidateTree();
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
		focusedObjects.size() == 1 ? focusedObjects[0] : nullptr,
		{ csl::math::Position{ *Desktop::instance->GetPickedLocation() }, csl::math::Position{ 0, 0, 0 } }
	};

	if (auto* objInfoAttribute = objectClassToPlace->GetAttribute("objinfo")) {
		auto* objInfoContainer = GameManager::GetInstance()->GetService<ObjInfoContainer>();
		auto* objInfoName = static_cast<const char*>(objInfoAttribute->m_Value);
		auto* objInfoClass = (*rangerssdk::GetAddress(&ObjInfoRegistry::instance))->objInfosByName.GetValueOrFallback(objInfoName, nullptr);
		auto* objInfo = objInfoClass->Create(GetAllocator());

		objInfoContainer->Register(objInfo->GetInfoName(), objInfo);
	}

	auto* rangeSpawningData = new (GetAllocator()) RangeSpawningData{ 50, 20 };
	auto* rangeSpawning = new (GetAllocator()) ComponentData{ GetAllocator(), GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName("RangeSpawning"), rangeSpawningData };
	objData->componentData.push_back(rangeSpawning);

	resource->AddObject(objData);
	focusedChunk->AddWorldObjectStatus(objData, true, 0);
	focusedObjects.clear();
	focusedObjects.push_back(objData);

	setObjectList.InvalidateTree();
}

void LevelEditor::Deselect()
{
	focusedObjects.clear();
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
	Deselect();
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
