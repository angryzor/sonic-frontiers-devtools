#pragma once
#include <debug-rendering/DebugRenderable.h>
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::svcol_editor {
    class SvColEditor : public OperationMode<Context>, public devtools::debug_rendering::DebugRenderable
    {
        bool renderKdTree{};
        bool renderBoundingVolumes{ false };
        bool renderCollisionShapes{ true };

    public:
        SvColEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host);

        virtual void ProcessAction(const ActionBase& action) override;
        virtual void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) override;
        virtual void RenderScene() override;
    };
}
