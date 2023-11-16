#pragma once
#include "../OperationMode.h"
#include "ObjectList.h"
#include "ObjectInspector.h"
#include "../../imgui/ImGuizmo.h"

class Desktop;
class ObjectInspection : public OperationMode {
    class ViewerContextHolder : public hh::fnd::ReferencedObject {
    public:
        csl::ut::MoveArray<hh::dbg::ViewerContext*> viewerContext{ GetAllocator() };
        ViewerContextHolder(csl::fnd::IAllocator* allocator);
    };
    class DebuggerThingEmulator : public hh::fnd::ReferencedObject {
    public:
        void* unk[9];
        hh::fnd::Reference<ViewerContextHolder> ctxHolder;
        DebuggerThingEmulator(csl::fnd::IAllocator* allocator);

        static DebuggerThingEmulator* instance;
    };

    ObjectList objectList{ GetAllocator(), *this };
    ObjectInspector objectInspector{ GetAllocator(), *this };
    ImGuizmo::OPERATION gizmoOperation{ ImGuizmo::TRANSLATE };
    ImGuizmo::MODE gizmoMode{ ImGuizmo::LOCAL };
    hh::fnd::Reference<hh::game::GameViewerContext> gameViewerCtx;
    hh::fnd::Reference<hh::game::ObjectViewerContext> objViewerCtx;
    hh::fnd::Reference<DebuggerThingEmulator> dbgEmulator;
    hh::fnd::Reference<hh::game::MousePickingViewer> picker;

public:
    Desktop& desktop;
    ObjectInspection(csl::fnd::IAllocator* allocator, Desktop& desktop);

    hh::fnd::Reference<hh::game::GameObject> focusedObject;

    void Render();
};
