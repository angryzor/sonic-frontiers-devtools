#pragma once
#include <ui/operation-modes/OperationMode.h>
#include <debug-rendering/GOCVisualDebugDrawRenderer.h>
#include "SetObjectList.h"
#include "ObjectDataInspector.h"
#include "ObjectLibrary.h"

class LevelEditor : public OperationMode, public hh::game::GameManagerListener, public hh::game::ObjectWorldListener, DebugRenderable {
    SetObjectList setObjectList{ GetAllocator(), *this };
    ObjectDataInspector objectDataInspector{ GetAllocator(), *this };
    ObjectLibrary objectLibrary{ GetAllocator(), *this };
    ImGuizmo::OPERATION gizmoOperation{ ImGuizmo::TRANSLATE };
    ImGuizmo::MODE gizmoMode{ ImGuizmo::LOCAL };
    bool haveSelectionAabb{ false };
    csl::geom::Aabb selectionAabb{};
    std::mt19937_64 mt;

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
    void SpawnObject();
    void Deselect();
    void DeleteFocusedObject();
    void ReloadObjectWorldData();

    void HandleObjectSelection();
    void HandleObjectManipulation();
    void CheckGizmoHotkeys();
    void CheckSelectionHotkeys();

    void RenderDebugComments();
};
