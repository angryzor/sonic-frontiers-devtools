#pragma once
#include "../OperationMode.h"
#include "SetObjectList.h"
#include "ObjectDataInspector.h"
#include "ObjectLibrary.h"
#include "../../imgui/ImGuizmo.h"

class LevelEditor : public OperationMode, public hh::game::GameManagerListener, public hh::game::ObjectWorldListener {
    SetObjectList setObjectList{ GetAllocator(), *this };
    ObjectDataInspector objectDataInspector{ GetAllocator(), *this };
    ObjectLibrary objectLibrary{ GetAllocator(), *this };
    ImGuizmo::OPERATION gizmoOperation{ ImGuizmo::TRANSLATE };
    ImGuizmo::MODE gizmoMode{ ImGuizmo::LOCAL };

public:
    LevelEditor(csl::fnd::IAllocator* allocator);
    virtual ~LevelEditor();

    hh::game::ObjectWorldChunk* focusedChunk{};
    hh::game::ObjectData* focusedObject{};
    const hh::game::GameObjectClass* objectClassToPlace{};

    virtual void Render() override;
    virtual void GameServiceAddedCallback(hh::game::GameService* gameService) override;
    virtual void GameServiceRemovedCallback(hh::game::GameService* gameService) override;
    virtual void WorldChunkRemovedCallback(hh::game::ObjectWorldChunk* chunk) override;

    void SetFocusedChunk(hh::game::ObjectWorldChunk* chunk);
};
