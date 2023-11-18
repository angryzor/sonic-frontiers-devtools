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
    hh::dbg::ViewerManager viewerManagerEmulator{ GetAllocator() };
    hh::fnd::Reference<hh::game::GameViewerContext> gameViewerCtx;
    hh::fnd::Reference<hh::game::ObjectViewerContext> objViewerCtx;
    hh::fnd::Reference<hh::physics::PhysicsViewerContext> physicsViewerCtx;
    //hh::fnd::Reference<hh::game::MousePickingViewer> picker;
    hh::fnd::Reference<hh::physics::PhysicsMousePickingViewer> physicsPicker;
    //hh::fnd::Reference<hh::physics::PhysicsPickedObjectViewer> physicsObjPicker;
    bool prevPhysicsPickerMouseDown;

public:
    Desktop& desktop;
    ObjectInspection(csl::fnd::IAllocator* allocator, Desktop& desktop);

    hh::fnd::Reference<hh::game::GameObject> focusedObject;

    void Render();
};
