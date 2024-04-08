#pragma once
#include <ui/operation-modes/OperationMode.h>
#include "ObjectList.h"
#include "ObjectInspector.h"

class ObjectInspection : public OperationMode {
    ObjectList objectList{ GetAllocator(), *this };
    ObjectInspector objectInspector{ GetAllocator(), *this };
    ImGuizmo::OPERATION gizmoOperation{ ImGuizmo::TRANSLATE };
    ImGuizmo::MODE gizmoMode{ ImGuizmo::LOCAL };

public:
    ObjectInspection(csl::fnd::IAllocator* allocator);

    hh::fnd::Reference<hh::game::GameObject> focusedObject{};

    virtual void Render() override;

    void DeleteFocusedObject();
    void HandleObjectSelection();
    void HandleObjectManipulation();
    void CheckGizmoHotkeys();
    void CheckSelectionHotkeys();
};
