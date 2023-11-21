#pragma once
#include "../OperationMode.h"
#include "SetObjectList.h"
#include "ObjectDataInspector.h"
#include "../../imgui/ImGuizmo.h"

class LevelEditor : public OperationMode {
    SetObjectList setObjectList{ GetAllocator(), *this };
    ObjectDataInspector objectDataInspector{ GetAllocator(), *this };
    ImGuizmo::OPERATION gizmoOperation{ ImGuizmo::TRANSLATE };
    ImGuizmo::MODE gizmoMode{ ImGuizmo::LOCAL };

public:
    LevelEditor(csl::fnd::IAllocator* allocator);

    hh::game::ObjectData* focusedObject{};

    virtual void Render() override;
};
