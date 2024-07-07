#include "LevelEditor.h"
#include <utilities/ObjectDataUtils.h>
#include <utilities/BoundingBoxes.h>
#include <ui/operation-modes/behaviors/Clipboard.h>
#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionMousePicking.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/SelectionVisual.h>
#include <ui/operation-modes/behaviors/Gizmo.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/Delete.h>

using namespace hh::fnd;
using namespace hh::game;

namespace app::gfx {
	class GOCVisualGeometryInstance : public GOComponent {
#ifdef DEVTOOLS_TARGET_SDK_wars
		GOCOMPONENT_CLASS_DECLARATION_INLINE_GETCLASS(GOCVisualGeometryInstance)
#else
		GOCOMPONENT_CLASS_DECLARATION(GOCVisualGeometryInstance)
#endif
	};
}

LevelEditor::LevelEditor(csl::fnd::IAllocator* allocator) : OperationMode{ allocator } {
	AddBehavior<SelectionBehavior<ObjectData*>>();
	AddBehavior<SelectionAabbBehavior<ObjectData*>>(*this);
	AddBehavior<SelectionTransformationBehavior<ObjectData*>>(*this);
	AddBehavior<SelectionVisual3DBehavior>();
	AddBehavior<GizmoBehavior>();
	AddBehavior<MousePickingPhysicsBehavior<ObjectData*>>(*this);
	AddBehavior<SelectionMousePickingBehavior<ObjectData*>>();
	AddBehavior<DeleteBehavior<ObjectData*>>(*this);
	AddBehavior<ClipboardBehavior<ObjectData*>>(*this);
	AddBehavior<ObjectLocationVisual3DBehavior<ObjectData*>>(*this);

	auto* gameManager = GameManager::GetInstance();

	gameManager->AddListener(this);

	if (auto* objWorld = gameManager->GetService<ObjectWorld>())
		objWorld->AddWorldListener(this);
}

LevelEditor::~LevelEditor() {
	SetFocusedChunk(nullptr);

	auto* gameManager = GameManager::GetInstance();

	gameManager->RemoveListener(this);

	if (auto* objWorld = gameManager->GetService<ObjectWorld>())
		objWorld->RemoveWorldListener(this);
}

void LevelEditor::Render() {
	setObjectList.Render();
	objectDataInspector.Render();
	objectLibrary.Render();
}

ObjectId LevelEditor::GenerateRandomObjectId() {
#ifdef DEVTOOLS_TARGET_SDK_wars
	return { mt() };
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
	return { mt(), mt() };
#endif
}

// This is extremely unlikely to happen with V3 object IDs and I would probably not even bother if we were only handling those,
// but it is less unlikely to happen with V2 object IDs (though still unlikely). 
ObjectId LevelEditor::GenerateUniqueRandomObjectId() {
	ObjectId result{};
	boolean isUnique{ true };

	// This has the potential to infinitely loop if the user has 2^32 objects in one chunk (V2).
	// It also has the potential to take a long time in collisions if there are less but still an extreme amount of
	// object IDs on the map.
	// I'm currently ignoring this possibility, but the second one could be avoided by only rolling once and then simply incrementing until we find a gap.
	// I just prefer a more random distribution, and we'll see if issues happen in practice. (Why have tests when you have end users :) )
	do {
		isUnique = true;
		result = GenerateRandomObjectId();

		for (auto& status : focusedChunk->GetObjectStatuses()) {
			if (status.objectData->id == result) {
				isUnique = false;
			}
		}
	} while (!result.IsNonNull() || !isUnique);

	return result;
}

void LevelEditor::RecalculateDependentTransforms()
{
	for (auto* obj : GetBehavior<SelectionBehavior<ObjectData*>>()->GetSelection())
		RecalculateDependentTransforms(obj);
}

void LevelEditor::RecalculateDependentTransforms(hh::game::ObjectData* objectData)
{
	// Copy changes to the live object if any.
	if (auto* obj = focusedChunk->GetGameObjectByObjectId(objectData->id))
		if (auto* gocTransform = obj->GetComponent<GOCTransform>())
			UpdateGOCTransform(*objectData, *gocTransform);

	for (auto* layer : focusedChunk->GetLayers()) {
		for (auto* child : layer->GetResource()->GetObjects()) {
			if (child->parentID == objectData->id) {
				RecalculateAbsoluteTransform(*objectData, *child);
				RecalculateDependentTransforms(child);
			}
		}
	}
}

ObjectData* LevelEditor::CreateObject(csl::fnd::IAllocator* allocator, const GameObjectClass* gameObjectClass, ObjectTransformData transform, ObjectData* parentObject) {
#ifdef DEVTOOLS_TARGET_SDK_wars
	// FIXME: We're leaking memory here because the string does not get freed.
	auto name = new char[100];
	snprintf(name, 100, "%s_%d", gameObjectClass->name, mt() % 0x1000000);
#else
	char name[100];
	snprintf(name, 100, "%s_%zd", gameObjectClass->name, mt() % 0x1000000);
#endif

	auto* objData = new (allocator) ObjectData{
		allocator,
		gameObjectClass,
		GenerateUniqueRandomObjectId(),
		name,
		parentObject,
		transform,
	};

	objData->componentData.push_back(ComponentData::Create(allocator, "RangeSpawning", GOCActivatorSpawner{ 500, 200 }));

	return objData;
}

ObjectData* LevelEditor::CopyObject(csl::fnd::IAllocator* allocator, ObjectData* otherObject) {
#ifdef DEVTOOLS_TARGET_SDK_wars
	// FIXME: We're leaking memory here because the string does not get freed.
	auto name = new char[100];
	snprintf(name, 100, "%s_%d", otherObject->gameObjectClass, mt() % 0x1000000);
#else
	char name[100];
	snprintf(name, 100, "%s_%zd", otherObject->gameObjectClass, mt() % 0x1000000);
#endif

	auto* objData = new (allocator) ObjectData{
		allocator,
		GenerateUniqueRandomObjectId(),
		name,
		*otherObject,
	};

	auto* otherRangeSpawning = otherObject->GetComponentDataByType("RangeSpawning");
	objData->componentData.push_back(ComponentData::Create(allocator, "RangeSpawning", otherRangeSpawning ? *otherRangeSpawning->GetData<GOCActivatorSpawner>() : GOCActivatorSpawner{ 500, 200 }));

	return objData;
}

ClipboardEntry<ObjectData*> LevelEditor::CreateClipboardEntry(ObjectData* objData) {
	return { *this, objData };
}

ObjectData* LevelEditor::CreateObject(const ClipboardEntry<ObjectData*>& entry) {
	return { CopyObject(placeTargetLayer->GetAllocator(), entry.proto) };
}

bool LevelEditor::HasTransform(ObjectData* obj) {
	return true;
}

bool LevelEditor::IsRoot(ObjectData* obj) {
	return !obj->parentID.IsNonNull();
}

ObjectData* LevelEditor::GetParent(ObjectData* obj) {
	return focusedChunk->GetWorldObjectStatusByObjectId(obj->parentID).objectData;
}

Eigen::Affine3f LevelEditor::GetSelectionSpaceTransform(ObjectData* obj) {
	return ObjectTransformDataToAffine3f(obj->transform);
}

void LevelEditor::SetSelectionSpaceTransform(ObjectData* obj, const Eigen::Affine3f& transform) {
	UpdateAbsoluteTransform(transform, *obj);
	RecalculateDependentTransforms(obj);
}

ObjectData* LevelEditor::GetObjectForGameObject(GameObject* obj) {
	return obj->GetWorldObjectStatus()->objectData;
}

bool LevelEditor::IsSelectable(hh::game::GameObject* obj)
{
	if (!focusedChunk)
		return false;

	auto* status = obj->GetWorldObjectStatus();

	return status != nullptr && focusedChunk->GetObjectIndexByObjectData(status->objectData) != -1;
}

const char* LevelEditor::GetObjectName(ObjectData* obj) {
	return obj->name;
}

void LevelEditor::GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<ObjectData*>& results) {
	if (!focusedChunk)
		return;

	for (auto* layer : focusedChunk->GetLayers())
		for (auto* object : layer->GetResource()->GetObjects())
			if (frustum.Test(object->transform.position))
				results.push_back(object);
}

bool LevelEditor::CalculateAabb(const csl::ut::MoveArray<hh::game::ObjectData*>& objects, csl::geom::Aabb& aabb)
{
	return CalcApproxAabb(focusedChunk, objects, aabb);
}

void LevelEditor::GetInvisibleObjects(csl::ut::MoveArray<ObjectData*>& objects) const
{
	if (!focusedChunk)
		return;

	for (auto& status : focusedChunk->GetObjectStatuses()) {
		auto* gameObject = focusedChunk->GetGameObjectByObjectId(status.objectData->id);

		if (!gameObject || ((!gameObject->GetComponent<hh::gfx::GOCVisual>() || gameObject->GetComponent<hh::gfx::GOCVisual>() == gameObject->GetComponent<hh::gfx::GOCVisualDebugDraw>()) && !gameObject->GetComponent<app::gfx::GOCVisualGeometryInstance>()))
			objects.push_back(status.objectData);
	}
}

Eigen::Affine3f LevelEditor::GetWorldTransform(ObjectData* object) const
{
	return ObjectTransformDataToAffine3f(object->transform);
}

void LevelEditor::DeleteObjects(const csl::ut::MoveArray<ObjectData*>& objects) {
	for (auto* obj : objects)
		focusedChunk->Despawn(obj);

	focusedChunk->ShutdownPendingObjects();

	for (auto* obj : objects)
		for (auto* layer : focusedChunk->GetLayers())
			for (auto* object : layer->GetResource()->GetObjects())
				if (object == obj)
					layer->RemoveObjectData(obj);
}

void LevelEditor::NotifySelectedObject()
{
	hh::dbg::MsgStartActiveObjectInEditor msg{};
	NotifyActiveObject(msg);
}

void LevelEditor::NotifyUpdatedObject()
{
	hh::dbg::MsgUpdateActiveObjectInEditor msg{};
	NotifyActiveObject(msg);

	//for (auto* obj : GameManager::GetInstance()->objects) {
	//	if (auto* grind = obj->GetComponent<app::game::GOCGrind>()) {
	//		hh::dbg::MsgUpdateActiveObjectInEditor msg;
	//		obj->SendMessageImm(msg);
	//		void* updater = *reinterpret_cast<void**>(reinterpret_cast<size_t>(grind) + 168);
	//		uint8_t* flag = reinterpret_cast<uint8_t*>(reinterpret_cast<size_t>(updater) + 172);
	//		*flag |= 2;
	//	}
	//}
}

void LevelEditor::NotifyDeselectedObject()
{
	hh::dbg::MsgFinishActiveObjectInEditor msg{};
	NotifyActiveObject(msg);
}

void LevelEditor::NotifyActiveObject(Message& msg)
{
	auto selection = GetBehavior<SelectionBehavior<ObjectData*>>()->GetSelection();
	if (selection.size() == 1) {
		if (auto* obj = focusedChunk->GetGameObjectByObjectId(selection[0]->id)) {
			obj->SendMessageImm(msg);
		}
	}
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
	if (focusedChunk == chunk) {
		GetBehavior<ClipboardBehavior<ObjectData*>>()->Clear();
		GetBehavior<SelectionBehavior<ObjectData*>>()->DeselectAll();
		placeTargetLayer = nullptr;
		objectClassToPlace = nullptr;
		focusedChunk = nullptr;
	}
}

void LevelEditor::SetFocusedChunk(ObjectWorldChunk* chunk)
{
	if (focusedChunk == chunk)
		return;

	GetBehavior<ClipboardBehavior<ObjectData*>>()->Clear();
	placeTargetLayer = nullptr;
	objectClassToPlace = nullptr;
	GetBehavior<SelectionBehavior<ObjectData*>>()->DeselectAll();
	if (focusedChunk) {
		focusedChunk->DespawnAll();
		focusedChunk->ShutdownPendingObjects();
		focusedChunk->SetEditorStatus(false);
		focusedChunk->SpawnByAttribute(GetStatusEternal);
	}
	focusedChunk = chunk;
	if (focusedChunk) {
		focusedChunk->SetEditorStatus(true);
		focusedChunk->DespawnAll();
		focusedChunk->ShutdownPendingObjects();
		focusedChunk->Restart(true);
		focusedChunk->SpawnByAttribute(GetStatusEternal);
	}
	setObjectList.InvalidateTree();
}
