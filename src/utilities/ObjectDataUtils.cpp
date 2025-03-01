#include "Pch.h"
#include "ObjectDataUtils.h"
#include <utilities/math/MathUtils.h>
#include <utilities/GameObjectUtils.h>

namespace ow2 = ucsl::resources::object_world::v2;
namespace ow3 = ucsl::resources::object_world::v3;

using ObjectDataV2 = ow2::ObjectData<he2sdk::ucsl::AllocatorSystem>;
using ObjectDataV3 = ow3::ObjectData<he2sdk::ucsl::AllocatorSystem>;

hh::game::ObjectTransformData Affine3fToObjectTransformData(const Eigen::Affine3f& transform) {
	Eigen::Matrix3f absoluteRotation;
	Eigen::Matrix3f absoluteScaling;

	transform.computeRotationScaling(&absoluteRotation, &absoluteScaling);

	return { { csl::math::Position{ transform.translation() }, csl::math::Position{ MatrixToEuler(absoluteRotation) } } };
}

Eigen::Affine3f ObjectTransformDataToAffine3f(const hh::game::ObjectTransformData& transformData) {
	Eigen::Affine3f transform;

	transform.fromPositionOrientationScale(transformData.position, EulerToQuat(transformData.rotation), csl::math::Vector3{ 1.0f, 1.0f, 1.0f });

	return transform;
}

void RecalculateLocalTransform(const Eigen::Affine3f& parentTransform, hh::game::ObjectData& objData) {
	auto absoluteTransform = ObjectTransformDataToAffine3f(objData.transform);

	objData.localTransform = Affine3fToObjectTransformData(parentTransform.inverse() * absoluteTransform);
}

void RecalculateAbsoluteTransform(const Eigen::Affine3f& parentTransform, hh::game::ObjectData& objData) {
	auto localTransform = ObjectTransformDataToAffine3f(objData.localTransform);

	objData.transform = Affine3fToObjectTransformData(parentTransform * localTransform);
}

void RecalculateLocalTransform(const hh::game::ObjectTransformData& parentTransform, hh::game::ObjectData& objData) {
	RecalculateLocalTransform(ObjectTransformDataToAffine3f(parentTransform), objData);
}

void RecalculateAbsoluteTransform(const hh::game::ObjectTransformData& parentTransform, hh::game::ObjectData& objData) {
	RecalculateAbsoluteTransform(ObjectTransformDataToAffine3f(parentTransform), objData);
}

void RecalculateLocalTransform(const hh::game::ObjectData& parent, hh::game::ObjectData& objData) {
	RecalculateLocalTransform(parent.transform, objData);
}

void RecalculateAbsoluteTransform(const hh::game::ObjectData& parent, hh::game::ObjectData& objData) {
	RecalculateAbsoluteTransform(parent.transform, objData);
}

void UpdateAbsoluteTransform(const Eigen::Affine3f& newAbsoluteTransform, hh::game::ObjectData& objData) {
	auto absoluteTransform = ObjectTransformDataToAffine3f(objData.transform);
	auto localTransform = ObjectTransformDataToAffine3f(objData.localTransform);
	auto parentTransform = absoluteTransform * localTransform.inverse();

	objData.transform = Affine3fToObjectTransformData(newAbsoluteTransform);
	objData.localTransform = Affine3fToObjectTransformData(parentTransform.inverse() * newAbsoluteTransform);
}

void UpdateLocalTransform(const Eigen::Affine3f& newLocalTransform, hh::game::ObjectData& objData) {
	auto absoluteTransform = ObjectTransformDataToAffine3f(objData.transform);
	auto localTransform = ObjectTransformDataToAffine3f(objData.localTransform);
	auto parentTransform = absoluteTransform * localTransform.inverse();

	objData.transform = Affine3fToObjectTransformData(parentTransform * newLocalTransform);
	objData.localTransform = Affine3fToObjectTransformData(newLocalTransform);
}

void UpdateGOCTransform(hh::game::ObjectData& objData, hh::game::GOCTransform& gocTransform) {
	// Depending on whether the parent was able to be spawned, the object uses the local or the absolute transform as the GOC transform, so we have to replicate that here.
	if (gocTransform.IsExistParent())
		gocTransform.SetLocalTransform({ objData.localTransform.position, EulerToQuat(objData.localTransform.rotation), csl::math::Vector3{ 1.0f, 1.0f, 1.0f } });
	else
		gocTransform.SetLocalTransform({ objData.transform.position, EulerToQuat(objData.transform.rotation), csl::math::Vector3{ 1.0f, 1.0f, 1.0f } });

	EnsureGOCTransformIsUpdating(gocTransform);
}

void* ConstructAndDeepCopy(csl::fnd::IAllocator* allocator, const hh::fnd::RflClass* rflClass) {
    auto* tinfoReg = hh::fnd::BuiltinTypeRegistry::GetTypeInfoRegistry();
    auto* rflObj = tinfoReg->ConstructObject(allocator, rflClass->GetName());

    void* result = hh::fnd::DeepCopier::Copy(rflObj, *rflClass, allocator);

    tinfoReg->CleanupLoadedObject(rflObj, rflClass->GetName());
    allocator->Free(rflObj);

    return result;
}

void* ConstructSpawnerData(csl::fnd::IAllocator* allocator, const hh::game::GameObjectClass* gameObjectClass) {
    auto* spawnerRflClass = gameObjectClass->GetSpawnerDataClass();

    if (spawnerRflClass == nullptr)
        return nullptr;

    return ConstructAndDeepCopy(allocator, spawnerRflClass);
}

void* CopySpawnerData(csl::fnd::IAllocator* allocator, const hh::game::GameObjectClass* gameObjectClass, const void* data) {
    auto* spawnerRflClass = gameObjectClass->GetSpawnerDataClass();

    if (spawnerRflClass == nullptr)
        return nullptr;

    return hh::fnd::DeepCopier::Copy(data, *spawnerRflClass, allocator);
}

ow2::ComponentData* CreateComponentDataV2(csl::fnd::IAllocator* allocator, const char* type, const hh::fnd::RflTypeInfo* rflTypeInfo) {
    void* buffer = new (std::align_val_t(16), allocator) char[rflTypeInfo->GetSize() + sizeof(ow2::ComponentData)];
    void* data = reinterpret_cast<void*>(reinterpret_cast<size_t>(buffer) + sizeof(ow2::ComponentData));

    rflTypeInfo->ConstructObject(data, allocator);

    return new (buffer) ow2::ComponentData{
		.unk1 = 0,
		.type = type,
		.size = rflTypeInfo->GetSize(),
		.data = data,
	};
}

ow3::ComponentData* CreateComponentDataV3(csl::fnd::IAllocator* allocator, const char* type, const hh::fnd::RflTypeInfo* rflTypeInfo) {
    void* buffer = new (std::align_val_t(16), allocator) char[rflTypeInfo->GetSize() + sizeof(ow3::ComponentData)];
    void* data = reinterpret_cast<void*>(reinterpret_cast<size_t>(buffer) + sizeof(ow3::ComponentData));
            
    rflTypeInfo->ConstructObject(data, allocator);

    return new (buffer) ow3::ComponentData{
		.unk1 = 0,
		.type = type,
		.size = rflTypeInfo->GetSize(),
		.data = data,
	};
}

ow2::ComponentData* CreateComponentDataV2(csl::fnd::IAllocator* allocator, const hh::game::GOComponentRegistry::GOComponentRegistryItem* goComponentRegistryItem) {
    return CreateComponentDataV2(allocator, goComponentRegistryItem->GetName(), hh::fnd::BuiltinTypeRegistry::GetTypeInfoRegistry()->GetTypeInfo(goComponentRegistryItem->GetSpawnerDataClass()->GetName()));
}

ow3::ComponentData* CreateComponentDataV3(csl::fnd::IAllocator* allocator, const hh::game::GOComponentRegistry::GOComponentRegistryItem* goComponentRegistryItem) {
    return CreateComponentDataV3(allocator, goComponentRegistryItem->GetName(), hh::fnd::BuiltinTypeRegistry::GetTypeInfoRegistry()->GetTypeInfo(goComponentRegistryItem->GetSpawnerDataClass()->GetName()));
}

ow2::ComponentData* CreateComponentDataV2(csl::fnd::IAllocator* allocator, const char* type) {
    return CreateComponentDataV2(allocator, hh::game::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(type));
}

ow3::ComponentData* CreateComponentDataV3(csl::fnd::IAllocator* allocator, const char* type) {
    return CreateComponentDataV3(allocator, hh::game::GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(type));
}

ObjectDataV2* CreateObjectDataV2(csl::fnd::IAllocator* allocator, const hh::game::GameObjectClass* gameObjectClass, ucsl::objectids::ObjectIdV1 id, const char* name, ObjectDataV2* parent, const ow2::ObjectTransformData& localTransform) {
    csl::ut::Bitset<ObjectDataV2::Flag> flags{};

    flags.set(ObjectDataV2::Flag::DEALLOCATE);

    void* spawnerData = ConstructSpawnerData(allocator, gameObjectClass);

    if (spawnerData)
        flags.set(ObjectDataV2::Flag::DEALLOCATE_SPAWNER_DATA);

    return new (allocator) ObjectDataV2 {
        .flags = flags,
        .gameObjectClass = gameObjectClass->GetName(),
        .name = name,
        .id = id,
        .parentID = parent ? parent->id : ucsl::objectids::ObjectIdV1{},
        .transform = parent ? (ow2::ObjectTransformData&&)Affine3fToObjectTransformData(ObjectTransformDataToAffine3f((hh::game::ObjectTransformData&)parent->transform) * ObjectTransformDataToAffine3f((hh::game::ObjectTransformData&)localTransform)) : localTransform,
        .localTransform = localTransform,
        .componentData = { allocator },
        .spawnerData = spawnerData,
    };
}

ObjectDataV3* CreateObjectDataV3(csl::fnd::IAllocator* allocator, const hh::game::GameObjectClass* gameObjectClass, ucsl::objectids::ObjectIdV2 id, const char* name, ObjectDataV3* parent, const ow3::ObjectTransformData& localTransform) {
    csl::ut::Bitset<ObjectDataV3::Flag> flags{};

    flags.set(ObjectDataV3::Flag::DEALLOCATE);

    void* spawnerData = ConstructSpawnerData(allocator, gameObjectClass);

    if (spawnerData)
        flags.set(ObjectDataV3::Flag::DEALLOCATE_SPAWNER_DATA);

    return new (allocator) ObjectDataV3 {
        .flags = flags,
        .gameObjectClass = gameObjectClass->GetName(),
        .name = { name, allocator },
        .id = id,
        .parentID = parent ? parent->id : ucsl::objectids::ObjectIdV2{},
        .transform = parent ? (ow3::ObjectTransformData&&)Affine3fToObjectTransformData(ObjectTransformDataToAffine3f((hh::game::ObjectTransformData&)parent->transform) * ObjectTransformDataToAffine3f((hh::game::ObjectTransformData&)localTransform)) : localTransform,
        .localTransform = localTransform,
        .componentData = { allocator },
        .spawnerData = spawnerData,
    };
}

ObjectDataV2* CopyObjectDataV2(csl::fnd::IAllocator* allocator, ucsl::objectids::ObjectIdV1 id, const char* name, ObjectDataV2* other) {
    csl::ut::Bitset<ObjectDataV2::Flag> flags{};

    flags.set(ObjectDataV2::Flag::DEALLOCATE);

    auto* gameObjectClass = hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(other->gameObjectClass);
    auto* spawnerData = CopySpawnerData(allocator, gameObjectClass, other->spawnerData);

    if (spawnerData)
        flags.set(ObjectDataV2::Flag::DEALLOCATE_SPAWNER_DATA);

    return new (allocator) ObjectDataV2 {
        .flags = flags,
        .gameObjectClass = other->gameObjectClass,
        .name = name,
        .id = id,
        .parentID = other->parentID,
        .transform = other->transform,
        .localTransform = other->localTransform,
        .componentData = { allocator },
        .spawnerData = spawnerData,
    };
}

ObjectDataV3* CopyObjectDataV3(csl::fnd::IAllocator* allocator, ucsl::objectids::ObjectIdV2 id, const char* name, ObjectDataV3* other) {
    csl::ut::Bitset<ObjectDataV3::Flag> flags{};

    flags.set(ObjectDataV3::Flag::DEALLOCATE);

    auto* gameObjectClass = hh::game::GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(other->gameObjectClass);
    auto* spawnerData = CopySpawnerData(allocator, gameObjectClass, other->spawnerData);

    if (spawnerData)
        flags.set(ObjectDataV3::Flag::DEALLOCATE_SPAWNER_DATA);

    return new (allocator) ObjectDataV3 {
        .flags = flags,
        .gameObjectClass = other->gameObjectClass,
        .name = { name, allocator },
        .id = id,
        .parentID = other->parentID,
        .transform = other->transform,
        .localTransform = other->localTransform,
        .componentData = { allocator },
        .spawnerData = spawnerData,
    };
}

const char* GetObjectName(ucsl::resources::object_world::v2::ObjectData<he2sdk::ucsl::AllocatorSystem>* obj) {
	return obj->name;
}

const char* GetObjectName(ucsl::resources::object_world::v3::ObjectData<he2sdk::ucsl::AllocatorSystem>* obj) {
	return obj->name.c_str();
}
