#pragma once
#include <ui/operation-modes/OperationMode.h>
#include <debug-rendering/GOCVisualDebugDrawRenderer.h>
#include "SetObjectList.h"
#include "ObjectDataInspector.h"
#include "ObjectLibrary.h"

namespace app::game {
    class GOCGrind : public hh::game::GOComponent {
        GOCOMPONENT_CLASS_DECLARATION(GOCGrind)
    };
}

class LevelEditor : public OperationMode, public hh::game::GameManagerListener, public hh::game::ObjectWorldListener, DebugRenderable {
    SetObjectList setObjectList{ GetAllocator(), *this };
    ObjectDataInspector objectDataInspector{ GetAllocator(), *this };
    ObjectLibrary objectLibrary{ GetAllocator(), *this };
    hh::fnd::Reference<hh::gfnd::GraphicsGeometry> targetBox{};
    ImGuizmo::OPERATION gizmoOperation{ ImGuizmo::TRANSLATE };
    ImGuizmo::MODE gizmoMode{ ImGuizmo::LOCAL };
    bool haveSelectionAabb{ false };
    csl::geom::Aabb selectionAabb{};
    csl::ut::MoveArray<hh::game::ObjectData*> clipboard{ GetAllocator() };
#ifdef DEVTOOLS_TARGET_SDK_wars
    std::mt19937 mt;
#else
    std::mt19937_64 mt;
#endif
    static float debugBoxScale;

    void RecalculateDependentTransforms(hh::game::ObjectData* objectData);

public:
    LevelEditor(csl::fnd::IAllocator* allocator);
    virtual ~LevelEditor();
    virtual void RenderDebugVisuals(hh::gfnd::DrawContext& ctx) override;

    hh::game::ObjectWorldChunk* focusedChunk{};
    csl::ut::MoveArray<hh::game::ObjectData*> focusedObjects{ GetAllocator() };
    const hh::game::GameObjectClass* objectClassToPlace{};
    hh::game::ObjectWorldChunkLayer* placeTargetLayer{};
    bool renderDebugComments{ false };

    virtual void Render() override;
    virtual void GameServiceAddedCallback(hh::game::GameService* gameService) override;
    virtual void GameServiceRemovedCallback(hh::game::GameService* gameService) override;
    virtual void WorldChunkRemovedCallback(hh::game::ObjectWorldChunk* chunk) override;

    void SetFocusedChunk(hh::game::ObjectWorldChunk* chunk);
    hh::game::ObjectData* CreateObject(csl::fnd::IAllocator* allocator, const hh::game::GameObjectClass* objectClass, hh::game::ObjectTransformData transform, hh::game::ObjectData* parentObject = nullptr);
    hh::game::ObjectData* CopyObject(csl::fnd::IAllocator* allocator, hh::game::ObjectData* otherObject);
    void SpawnObject();
    void SpawnObject(hh::game::ObjectData* objectData);
    void Select(const csl::ut::MoveArray<hh::game::GameObject*>& objs);
    void Select(const csl::ut::MoveArray<hh::game::ObjectData*>& objectDatas);
    void Select(hh::game::GameObject* gameObject);
    void Select(hh::game::ObjectData* objectData);
    void Deselect();
    void NotifySelectedObject();
    void NotifyUpdatedObject();
    void NotifyDeselectedObject();
    void NotifyActiveObject(hh::fnd::Message& msg);
    void DeleteFocusedObject();
    void ReloadObjectWorldData();
    static void SetDebugBoxScale(float scale);
    void UpdateDebugBoxGeometry();

    void HandleObjectSelection();
    void HandleObjectManipulation();
    void CheckGizmoHotkeys();
    void CheckSelectionHotkeys();

    void RenderDebugComments();

    void RecalculateDependentTransforms();

    void ClearClipboard();
    void HandleClipboard();
    void HandleCopy();
    void HandlePaste();
};
