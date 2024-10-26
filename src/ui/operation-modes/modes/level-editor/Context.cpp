#include "Context.h"
#include <utilities/ObjectDataUtils.h>
#include <utilities/BoundingBoxes.h>

#ifdef DEVTOOLS_TARGET_SDK_rangers
namespace app::game {
	class GOCGrind : public hh::game::GOComponent {
		GOCOMPONENT_CLASS_DECLARATION(GOCGrind)
	};
}
#endif

namespace ui::operation_modes::modes::level_editor {
	using namespace hh::fnd;
	using namespace hh::game;

	Context::~Context() {
		SetFocusedChunk(nullptr);
	}

	ObjectId Context::GenerateRandomObjectId() {
#ifdef DEVTOOLS_TARGET_SDK_wars
		return { mt() };
#endif
#ifndef DEVTOOLS_TARGET_SDK_wars
		return { mt(), mt() };
#endif
	}

	// This is extremely unlikely to happen with V3 object IDs and I would probably not even bother if we were only handling those,
	// but it is less unlikely to happen with V2 object IDs (though still unlikely). 
	ObjectId Context::GenerateUniqueRandomObjectId() {
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

	void Context::RecalculateDependentTransforms(const csl::ut::MoveArray<ObjectData*>& objectDatas) {
		for (auto* obj : objectDatas)
			RecalculateDependentTransforms(obj);
	}

	void Context::NotifySelectedObject(hh::game::ObjectData* objectData)
	{
		hh::dbg::MsgStartActiveObjectInEditor msg{};
		NotifyObject(objectData, msg);
	}

	void Context::NotifyUpdatedObject(hh::game::ObjectData* objectData)
	{
		hh::dbg::MsgUpdateActiveObjectInEditor msg{};
		NotifyObject(objectData, msg);

	}

	void Context::NotifyDeselectedObject(hh::game::ObjectData* objectData)
	{
		hh::dbg::MsgFinishActiveObjectInEditor msg{};
		NotifyObject(objectData, msg);
	}

	void Context::NotifyParamChanged(hh::game::ObjectData* objectData)
	{
		hh::dbg::MsgParamChangedInEditor msg{};
		NotifyObject(objectData, msg);
	}

	void Context::NotifyObject(hh::game::ObjectData* objectData, hh::fnd::Message& msg)
	{
		if (auto* obj = focusedChunk->GetGameObject(objectData))
			obj->SendMessageImm(msg);
	}

	void Context::UpdateGrindRails()
	{
#ifdef DEVTOOLS_TARGET_SDK_rangers
		// Live update grind rails
		for (auto* obj : GameManager::GetInstance()->objects) {
			if (auto* grind = obj->GetComponent<app::game::GOCGrind>()) {
				hh::dbg::MsgUpdateActiveObjectInEditor msg;
				obj->SendMessageImm(msg);
				void* updater = *reinterpret_cast<void**>(reinterpret_cast<size_t>(grind) + 168);
				uint8_t* flag = reinterpret_cast<uint8_t*>(reinterpret_cast<size_t>(updater) + 172);
				*flag |= 2;
			}
		}
#endif
	}

	void Context::ReloadActiveObjectParameters(hh::game::ObjectData* objectData)
	{
		NotifyParamChanged(objectData);

		// The only reason I do this is because apparently sending MsgParamChanged sets the debug visual visibility to false on volumes??
		NotifySelectedObject(objectData);
		NotifyUpdatedObject(objectData);
	}

	// This is a hack to force the object to reload its parameters
	void Context::RespawnActiveObject(hh::game::ObjectData* objectData)
	{
		focusedChunk->Despawn(objectData);
		focusedChunk->ShutdownPendingObjects();
		focusedChunk->Spawn(objectData);
		NotifySelectedObject(objectData);
	}

	void Context::RecalculateDependentTransforms(hh::game::ObjectData* objectData) {
		// Copy changes to the live object if any.
		if (auto* obj = focusedChunk->GetGameObject(objectData))
			if (auto* gocTransform = obj->GetComponent<GOCTransform>())
				UpdateGOCTransform(*objectData, *gocTransform);

		// Some tools generate faulty objects with ID 0. Avoid crashing on those.
		if (!objectData->id.IsNonNull())
			return;

		for (auto* layer : focusedChunk->GetLayers()) {
			for (auto* child : layer->GetResource()->GetObjects()) {
				if (child->parentID == objectData->id) {
					RecalculateAbsoluteTransform(*objectData, *child);
					RecalculateDependentTransforms(child);
				}
			}
		}
	}

	ObjectData* Context::CreateObject(csl::fnd::IAllocator* allocator, const GameObjectClass* gameObjectClass, ObjectTransformData transform, ObjectData* parentObject) {
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

		auto* rangeSpawning = ComponentData::Create<GOCActivatorSpawner>(allocator, "RangeSpawning");
		auto* rangeSpawningData = rangeSpawning->GetData<GOCActivatorSpawner>();

		rangeSpawningData->m_range = 500.0f;
		rangeSpawningData->m_distance = 200.0f;

		objData->componentData.push_back(rangeSpawning);

		return objData;
	}

	ObjectData* Context::CopyObject(csl::fnd::IAllocator* allocator, ObjectData* otherObject) {
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
		auto* newRangeSpawning = ComponentData::Create<GOCActivatorSpawner>(allocator, "RangeSpawning");

		auto* newRangeSpawningData = newRangeSpawning->GetData<GOCActivatorSpawner>();
		
		if (otherRangeSpawning) {
			auto* otherRangeSpawningData = otherRangeSpawning->GetData<GOCActivatorSpawner>();
			*newRangeSpawningData = *otherRangeSpawningData;
		}
		else {
			newRangeSpawningData->m_range = 500.0f;
			newRangeSpawningData->m_distance = 200.0f;
		}

		objData->componentData.push_back(newRangeSpawning);

		return objData;
	}

	hh::game::ObjectData* Context::CopyObjectForPlacement(hh::game::ObjectData* otherObject)
	{
		return CopyObject(placementTargetLayer->GetAllocator(), otherObject);
	}

	hh::game::ObjectData* Context::CopyObjectForClipboard(hh::game::ObjectData* otherObject)
	{
		return CopyObject(GetAllocator(), otherObject);
	}

	void Context::DeleteObjects(const csl::ut::MoveArray<ObjectData*>& objects)
	{
		for (auto* obj : objects)
			focusedChunk->Despawn(obj);

		focusedChunk->ShutdownPendingObjects();

		for (auto* obj : objects)
			for (auto* layer : focusedChunk->GetLayers())
				for (auto* object : layer->GetResource()->GetObjects())
					if (object == obj)
						layer->RemoveObjectData(obj);
	}

	void Context::TerminateClipboardObject(hh::game::ObjectData* objectData)
	{
		TerminateObjectData(GetAllocator(), objectData);
	}

	hh::game::ObjectData* Context::SpawnObject(const csl::math::Vector3& position) {
		auto* result = CreateObject(placementTargetLayer->GetAllocator(), objectClassToPlace, { position, { 0.0f, 0.0f, 0.0f } }, nullptr);
		SpawnObject(result);
		return result;
	}

	void Context::SpawnObject(ObjectData* objData) {
		auto* resource = placementTargetLayer->GetResource();

		if (auto* objInfoName = static_cast<const char*>(hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(objData->gameObjectClass)->GetAttributeValue("objinfo"))) {
			auto* objInfoContainer = GameManager::GetInstance()->GetService<ObjInfoContainer>();
			auto* objInfoClass = RESOLVE_STATIC_VARIABLE(ObjInfoRegistry::instance)->objInfosByName.GetValueOrFallback(objInfoName, nullptr);
			auto* objInfo = objInfoClass->Create(GetAllocator());

			objInfoContainer->Register(objInfo->GetInfoName(), objInfo);
		}

		resource->AddObject(objData);
#ifdef DEVTOOLS_TARGET_SDK_miller
		focusedChunk->AddWorldObjectStatus(placementTargetLayer, objData, true);
#else	
		focusedChunk->AddWorldObjectStatus(objData, true);
#endif
	}

	hh::game::ObjectWorldChunk* Context::GetFocusedChunk() const
	{
		return focusedChunk;
	}

	void Context::SetFocusedChunk(hh::game::ObjectWorldChunk* chunk)
	{
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
		ResetPlacementState();
	}

	void Context::ReleaseChunk()
	{
		focusedChunk = nullptr;
		ResetPlacementState();
	}

	void Context::ResetPlacementState()
	{
		placementTargetLayer = nullptr;
		objectClassToPlace = nullptr;
	}

	void Context::SetObjectParent(hh::game::ObjectData* child, hh::game::ObjectData* parent)
	{
		auto parentAbsoluteTransform = ObjectTransformDataToAffine3f(parent->transform);
		auto childAbsoluteTransform = ObjectTransformDataToAffine3f(child->transform);

		child->localTransform = Affine3fToObjectTransformData(parentAbsoluteTransform.inverse() * childAbsoluteTransform);
		child->parentID = parent->id;

		focusedChunk->Despawn(child);
		focusedChunk->Restart(focusedChunk->GetObjectIndexByObjectData(child), true);
	}
}
