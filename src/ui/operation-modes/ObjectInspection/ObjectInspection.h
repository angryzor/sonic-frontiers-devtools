#pragma once
#include <debug-rendering/GOCVisualDebugDrawRenderer.h>
#include <ui/operation-modes/OperationMode.h>
#include "ObjectList.h"
#include "ObjectInspector.h"

class ObjectInspection : public OperationMode, DebugRenderable {
    ObjectList objectList{ GetAllocator(), *this };
    ObjectInspector objectInspector{ GetAllocator(), *this };
    ImGuizmo::OPERATION gizmoOperation{ ImGuizmo::TRANSLATE };
    ImGuizmo::MODE gizmoMode{ ImGuizmo::LOCAL };
    bool haveSelectionAabb{ false };
    csl::geom::Aabb selectionAabb{};

public:
    ObjectInspection(csl::fnd::IAllocator* allocator);

    csl::ut::MoveArray<hh::fnd::Reference<hh::game::GameObject>> focusedObjects{ GetAllocator() };

    virtual void Render() override;
    virtual void RenderDebugVisuals(hh::gfnd::DrawContext& ctx) override;

    void DeleteFocusedObject();
    void HandleObjectSelection();
    void HandleObjectManipulation();
    void CheckGizmoHotkeys();
    void CheckSelectionHotkeys();
    void Select(hh::game::GameObject* gameObject);
};
