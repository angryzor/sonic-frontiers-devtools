#pragma once
#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/Clipboard.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/MousePicking.h>
#include <ui/operation-modes/behaviors/Delete.h>
#include <ui/operation-modes/behaviors/ObjectLocationVisual3D.h>
#include "SetObjectList.h"
#include "ObjectDataInspector.h"
#include "ObjectLibrary.h"

class LevelEditor
    : public OperationMode
    , public hh::game::GameManagerListener
    , public hh::game::ObjectWorldListener
    , public ClipboardBehavior<hh::game::ObjectData*>::Operations
    , public SelectionTransformationBehavior<hh::game::ObjectData*>::Operations
    , public MousePickingPhysicsBehavior<hh::game::ObjectData*>::Operations
    , public DeleteBehavior<hh::game::ObjectData*>::Operations
    , public SelectionAabbBehavior<hh::game::ObjectData*>::Operations
    , public ObjectLocationVisual3DBehavior<hh::game::ObjectData*>::Operations
{
    SetObjectList setObjectList{ GetAllocator(), *this };
    ObjectDataInspector objectDataInspector{ GetAllocator(), *this };
    ObjectLibrary objectLibrary{ GetAllocator(), *this };
#ifdef DEVTOOLS_TARGET_SDK_wars
    std::mt19937 mt;
#else
    std::mt19937_64 mt;
#endif

    void RecalculateDependentTransforms(hh::game::ObjectData* objectData);
    hh::game::ObjectId GenerateRandomObjectId();
    hh::game::ObjectId GenerateUniqueRandomObjectId();

public:
    hh::game::ObjectWorldChunk* focusedChunk{};
    const hh::game::GameObjectClass* objectClassToPlace{};
    hh::game::ObjectWorldChunkLayer* placeTargetLayer{};

    LevelEditor(csl::fnd::IAllocator* allocator);
    virtual ~LevelEditor();

    virtual void Render() override;
    virtual void GameServiceAddedCallback(hh::game::GameService* gameService) override;
    virtual void GameServiceRemovedCallback(hh::game::GameService* gameService) override;
    virtual void WorldChunkRemovedCallback(hh::game::ObjectWorldChunk* chunk) override;

    hh::game::ObjectData* CreateObject(csl::fnd::IAllocator* allocator, const hh::game::GameObjectClass* objectClass, hh::game::ObjectTransformData transform, hh::game::ObjectData* parentObject = nullptr);
    hh::game::ObjectData* CopyObject(csl::fnd::IAllocator* allocator, hh::game::ObjectData* otherObject);

    void SetFocusedChunk(hh::game::ObjectWorldChunk* chunk);
    void NotifySelectedObject();
    void NotifyUpdatedObject();
    void NotifyDeselectedObject();
    void NotifyActiveObject(hh::fnd::Message& msg);
    void RecalculateDependentTransforms();

    virtual ClipboardEntry<hh::game::ObjectData*> CreateClipboardEntry(hh::game::ObjectData* objData) override;
    virtual hh::game::ObjectData* CreateObject(const ClipboardEntry<hh::game::ObjectData*>& entry) override;

    virtual bool HasTransform(hh::game::ObjectData* obj) override;
    virtual bool IsRoot(hh::game::ObjectData* obj) override;
    virtual hh::game::ObjectData* GetParent(hh::game::ObjectData* obj) override;
    virtual Eigen::Affine3f GetSelectionSpaceTransform(hh::game::ObjectData* obj) override;
    virtual void SetSelectionSpaceTransform(hh::game::ObjectData* obj, const Eigen::Affine3f& transform) override;

    virtual void DeleteObjects(const csl::ut::MoveArray<hh::game::ObjectData*>& obj) override;

    virtual hh::game::ObjectData* GetObjectForGameObject(hh::game::GameObject* obj) override;
    virtual bool IsSelectable(hh::game::GameObject* obj) override;
    virtual const char* GetObjectName(hh::game::ObjectData* obj) override;
    virtual void GetFrustumResults(const Frustum& frustum, csl::ut::MoveArray<hh::game::ObjectData*>& results) override;

    virtual bool CalculateAabb(const csl::ut::MoveArray<hh::game::ObjectData*>& objects, csl::geom::Aabb& aabb) override;

    virtual void GetInvisibleObjects(csl::ut::MoveArray<hh::game::ObjectData*>& objects) const override;
    virtual Eigen::Affine3f GetWorldTransform(hh::game::ObjectData* object) const override;
};

template<> struct ClipboardEntry<hh::game::ObjectData*> {
    LevelEditor& levelEditor;
    hh::game::ObjectData* proto;

    ClipboardEntry(LevelEditor& levelEditor, hh::game::ObjectData* proto) : levelEditor{ levelEditor }, proto{ levelEditor.CopyObject(levelEditor.GetAllocator(), proto) } {}
    ClipboardEntry(const ClipboardEntry& other) : levelEditor{ other.levelEditor }, proto{ levelEditor.CopyObject(levelEditor.GetAllocator(), other.proto) } {}
    ClipboardEntry(ClipboardEntry&& other) noexcept : levelEditor{ other.levelEditor }, proto{ other.proto } { other.proto = nullptr; }
    ~ClipboardEntry() { TerminateObjectData(levelEditor.GetAllocator(), proto); }
};
