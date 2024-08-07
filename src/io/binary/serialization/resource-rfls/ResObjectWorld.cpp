#include "ResObjectWorld.h"

#include <utilities/ObjectDataUtils.h>

using namespace hh::fnd;
using namespace hh::game;

//struct ComponentData {
//    uint64_t unk1;
//    const char* type;
//    uint64_t size;
//    void* data;
//};

//struct ObjectTransformData {
//    csl::math::Position position;
//    csl::math::Position rotation;
//};

//struct ObjectData {
//    uint32_t unk1;
//    uint32_t unk2;
//    const char* gameObjectClass;
//    csl::ut::VariableString name;
//    ObjectId id;
//    ObjectId parentID;
//    ObjectTransformData transform;
//    ObjectTransformData localTransform;
//    csl::ut::MoveArray<ComponentData*> componentData;
//    void* objInfo;
//};

//struct ObjectWorldData {
//    uint32_t unk1;
//    uint32_t unk2;
//    uint64_t unk3;
//    csl::ut::MoveArray<ObjectData*> objects;
//};

#ifdef DEVTOOLS_TARGET_SDK_wars
const RflClass* handleComponentDataData(void* parent) {
    const char* componentType = static_cast<ComponentData*>(parent)->type;

    if (!strcmp(componentType, "RangeSpawning"))
        return &GOCActivatorSpawner::rflClass;
    else
        return nullptr;
}
#else
const RflClass* handleComponentDataData(void* parent) {
    return GameObjectSystem::GetInstance()->goComponentRegistry->GetComponentInformationByName(static_cast<ComponentData*>(parent)->type)->rflClass;
}
#endif

const RflClass* handleSpawnerData(void* parent) {
    return GameObjectSystem::GetInstance()->gameObjectRegistry->GetGameObjectClassByName(static_cast<ObjectData*>(parent)->gameObjectClass)->spawnerDataRflClass;
}

RflClassMember componentDataMembers[]{
    { "unk1", nullptr, nullptr, RflClassMember::TYPE_UINT64, RflClassMember::TYPE_VOID, 0, 0x100000, offsetof(ComponentData, unk1), nullptr },
    { "type", nullptr, nullptr, RflClassMember::TYPE_CSTRING, RflClassMember::TYPE_VOID, 0, 0, offsetof(ComponentData, type), nullptr },
    { "size", nullptr, nullptr, RflClassMember::TYPE_UINT64, RflClassMember::TYPE_VOID, 0, 0, offsetof(ComponentData, size), nullptr },
    { "data", reinterpret_cast<RflClass*>(handleComponentDataData), nullptr, RflClassMember::TYPE_POINTER, RflClassMember::TYPE_STRUCT, 0, 0x10000, offsetof(ComponentData, data), nullptr },
};

RflClass componentData{ "ComponentData", nullptr, sizeof(ComponentData), nullptr, 0, componentDataMembers, 4, nullptr };

RflClassMember objectTransformDataMembers[] = {
    { "position", nullptr, nullptr, RflClassMember::TYPE_POSITION, RflClassMember::TYPE_VOID, 0, 0, offsetof(ObjectTransformData, position), nullptr },
    { "rotation", nullptr, nullptr, RflClassMember::TYPE_POSITION, RflClassMember::TYPE_VOID, 0, 0, offsetof(ObjectTransformData, rotation), nullptr },
};

RflClass objectTransformData{ "ObjectTransformData", nullptr, sizeof(ObjectTransformData), nullptr, 0, objectTransformDataMembers, 2, nullptr };

RflClassMember componentDataArrayEntryMembers[]{
    { "ptr", &componentData, nullptr, RflClassMember::TYPE_POINTER, RflClassMember::TYPE_STRUCT, 0, 0, 0, nullptr },
};

RflClass componentDataArrayEntry{ "ComponentDataArrayEntry", nullptr, 8, nullptr, 0, componentDataArrayEntryMembers, 1, nullptr };

RflClassMember objectDataMembers[]{
    { "flags", nullptr, nullptr, RflClassMember::TYPE_UINT32, RflClassMember::TYPE_VOID, 0, 0x100000, offsetof(ObjectData, flags), nullptr },
    { "gameObjectClass", nullptr, nullptr, RflClassMember::TYPE_CSTRING, RflClassMember::TYPE_VOID, 0, 0, offsetof(ObjectData, gameObjectClass), nullptr },
#ifdef DEVTOOLS_TARGET_SDK_wars
    { "name", nullptr, nullptr, RflClassMember::TYPE_CSTRING, RflClassMember::TYPE_VOID, 0, 0, offsetof(ObjectData, name), nullptr },
#else
    { "name", nullptr, nullptr, RflClassMember::TYPE_STRING, RflClassMember::TYPE_VOID, 0, 0, offsetof(ObjectData, name), nullptr },
#endif
    { "id", nullptr, nullptr, RflClassMember::TYPE_OBJECT_ID, RflClassMember::TYPE_VOID, 0, 0, offsetof(ObjectData, id), nullptr },
    { "parentID", nullptr, nullptr, RflClassMember::TYPE_OBJECT_ID, RflClassMember::TYPE_VOID, 0, 0, offsetof(ObjectData, parentID), nullptr },
    { "transform", &objectTransformData, nullptr, RflClassMember::TYPE_STRUCT, RflClassMember::TYPE_VOID, 0, 0, offsetof(ObjectData, transform), nullptr },
    { "localTransform", &objectTransformData, nullptr, RflClassMember::TYPE_STRUCT, RflClassMember::TYPE_VOID, 0, 0, offsetof(ObjectData, localTransform), nullptr },
    { "componentData", &componentDataArrayEntry, nullptr, RflClassMember::TYPE_ARRAY, RflClassMember::TYPE_STRUCT, 0, 0, offsetof(ObjectData, componentData), nullptr },
    { "spawnerData", reinterpret_cast<RflClass*>(handleSpawnerData), nullptr, RflClassMember::TYPE_POINTER, RflClassMember::TYPE_STRUCT, 0, 0x10000, offsetof(ObjectData, spawnerData), nullptr },
};

RflClass objectData{ "ObjectData", nullptr, sizeof(ObjectData), nullptr, 0, objectDataMembers, 9, nullptr };

RflClassMember objectDataArrayEntryMembers[]{
    { "ptr", &objectData, nullptr, RflClassMember::TYPE_POINTER, RflClassMember::TYPE_STRUCT, 0, 0, 0, nullptr },
};

RflClass objectDataArrayEntry{ "ObjectDataArrayEntry", nullptr, 8, nullptr, 0, objectDataArrayEntryMembers, 1, nullptr };

RflClassMember resObjWorldMembers[]{
    { "unk1", nullptr, nullptr, RflClassMember::TYPE_UINT32, RflClassMember::TYPE_VOID, 0, 0x100000, offsetof(ObjectWorldData, unk1), nullptr },
    { "unk2", nullptr, nullptr, RflClassMember::TYPE_UINT32, RflClassMember::TYPE_VOID, 0, 0x100000, offsetof(ObjectWorldData, unk2), nullptr },
    { "flags", nullptr, nullptr, RflClassMember::TYPE_UINT8, RflClassMember::TYPE_VOID, 0, 0x100000, offsetof(ObjectWorldData, flags), nullptr },
    { "objects", &objectDataArrayEntry, nullptr, RflClassMember::TYPE_ARRAY, RflClassMember::TYPE_STRUCT, 0, 0, offsetof(ObjectWorldData, objects), nullptr },
};

RflClass ResourceRfls::resObjectWorld{ "ObjectWorldData", nullptr, sizeof(ObjectWorldData), nullptr, 0, resObjWorldMembers, 4, nullptr };
