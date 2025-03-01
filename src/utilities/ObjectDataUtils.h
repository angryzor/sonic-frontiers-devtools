#pragma once
#include <ucsl/resources/object-world/v2.h>
#include <ucsl/resources/object-world/v3.h>

hh::game::ObjectTransformData Affine3fToObjectTransformData(const Eigen::Affine3f& transform);
Eigen::Affine3f ObjectTransformDataToAffine3f(const hh::game::ObjectTransformData& transformData);
void RecalculateLocalTransform(const Eigen::Affine3f& parentTransform, hh::game::ObjectData& objData);
void RecalculateAbsoluteTransform(const Eigen::Affine3f& parentTransform, hh::game::ObjectData& objData);
void RecalculateLocalTransform(const hh::game::ObjectTransformData& parentTransform, hh::game::ObjectData& objData);
void RecalculateAbsoluteTransform(const hh::game::ObjectTransformData& parentTransform, hh::game::ObjectData& objData);
void RecalculateLocalTransform(const hh::game::ObjectData& parent, hh::game::ObjectData& objData);
void RecalculateAbsoluteTransform(const hh::game::ObjectData& parent, hh::game::ObjectData& objData);
void UpdateAbsoluteTransform(const Eigen::Affine3f& newAbsoluteTransform, hh::game::ObjectData& objData);
void UpdateLocalTransform(const Eigen::Affine3f& newLocalTransform, hh::game::ObjectData& objData);
void UpdateGOCTransform(hh::game::ObjectData& objData, hh::game::GOCTransform& gocTransform);

void* ConstructAndDeepCopy(csl::fnd::IAllocator* allocator, const hh::fnd::RflClass* rflClass);
void* ConstructSpawnerData(csl::fnd::IAllocator* allocator, const hh::game::GameObjectClass* gameObjectClass);
void* CopySpawnerData(csl::fnd::IAllocator* allocator, const hh::game::GameObjectClass* gameObjectClass, const void* data);

ucsl::resources::object_world::v2::ComponentData* CreateComponentDataV2(csl::fnd::IAllocator* allocator, const char* type, const hh::fnd::RflTypeInfo* rflTypeInfo);
ucsl::resources::object_world::v3::ComponentData* CreateComponentDataV3(csl::fnd::IAllocator* allocator, const char* type, const hh::fnd::RflTypeInfo* rflTypeInfo);

ucsl::resources::object_world::v2::ComponentData* CreateComponentDataV2(csl::fnd::IAllocator* allocator, const hh::game::GOComponentRegistry::GOComponentRegistryItem* goComponentRegistryItem);
ucsl::resources::object_world::v3::ComponentData* CreateComponentDataV3(csl::fnd::IAllocator* allocator, const hh::game::GOComponentRegistry::GOComponentRegistryItem* goComponentRegistryItem);

ucsl::resources::object_world::v2::ComponentData* CreateComponentDataV2(csl::fnd::IAllocator* allocator, const char* type);
ucsl::resources::object_world::v3::ComponentData* CreateComponentDataV3(csl::fnd::IAllocator* allocator, const char* type);

template<typename T>
ucsl::resources::object_world::v2::ComponentData* CreateComponentDataV2(csl::fnd::IAllocator* allocator, const char* type) {
    return CreateComponentDataV2(allocator, type, &T::typeInfo);
}

template<typename T>
ucsl::resources::object_world::v3::ComponentData* CreateComponentDataV3(csl::fnd::IAllocator* allocator, const char* type) {
#ifdef DEVTOOLS_TARGET_SDK_miller
    return CreateComponentDataV3(allocator, type, &RESOLVE_STATIC_VARIABLE(T::typeInfo));
#else
    return CreateComponentDataV3(allocator, type, &T::typeInfo);
#endif
}

ucsl::resources::object_world::v2::ObjectData<he2sdk::ucsl::AllocatorSystem>* CreateObjectDataV2(
    csl::fnd::IAllocator* allocator,
    const hh::game::GameObjectClass* gameObjectClass,
    ucsl::objectids::ObjectIdV1 id,
    const char* name,
    ucsl::resources::object_world::v2::ObjectData<he2sdk::ucsl::AllocatorSystem>* parent,
    const ucsl::resources::object_world::v2::ObjectTransformData& localTransform
);
ucsl::resources::object_world::v3::ObjectData<he2sdk::ucsl::AllocatorSystem>* CreateObjectDataV3(
    csl::fnd::IAllocator* allocator,
    const hh::game::GameObjectClass* gameObjectClass,
    ucsl::objectids::ObjectIdV2 id,
    const char* name,
    ucsl::resources::object_world::v3::ObjectData<he2sdk::ucsl::AllocatorSystem>* parent,
    const ucsl::resources::object_world::v3::ObjectTransformData& localTransform
);

ucsl::resources::object_world::v2::ObjectData<he2sdk::ucsl::AllocatorSystem>* CopyObjectDataV2(
    csl::fnd::IAllocator* allocator,
    ucsl::objectids::ObjectIdV1 id,
    const char* name,
    ucsl::resources::object_world::v2::ObjectData<he2sdk::ucsl::AllocatorSystem>* other
);
ucsl::resources::object_world::v3::ObjectData<he2sdk::ucsl::AllocatorSystem>* CopyObjectDataV3(
    csl::fnd::IAllocator* allocator,
    ucsl::objectids::ObjectIdV2 id,
    const char* name,
    ucsl::resources::object_world::v3::ObjectData<he2sdk::ucsl::AllocatorSystem>* other
);

const char* GetObjectName(ucsl::resources::object_world::v2::ObjectData<he2sdk::ucsl::AllocatorSystem>* obj);
const char* GetObjectName(ucsl::resources::object_world::v3::ObjectData<he2sdk::ucsl::AllocatorSystem>* obj);
