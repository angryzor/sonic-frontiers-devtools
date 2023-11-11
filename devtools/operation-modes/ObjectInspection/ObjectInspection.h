#pragma once
#include "../OperationMode.h"
#include "ObjectList.h"
#include "ObjectInspector.h"
#include "../../imgui/ImGuizmo.h"

class Desktop;
class ObjectInspection : public OperationMode {
    ObjectList objectList{ GetAllocator(), *this };
    ObjectInspector objectInspector{ GetAllocator(), *this };
    ImGuizmo::OPERATION gizmoOperation{ ImGuizmo::TRANSLATE };
    ImGuizmo::MODE gizmoMode{ ImGuizmo::LOCAL };

public:
    Desktop& desktop;
    ObjectInspection(csl::fnd::IAllocator* allocator, Desktop& desktop);

    hh::fnd::Reference<hh::game::GameObject> focusedObject;

    void Render();
};
