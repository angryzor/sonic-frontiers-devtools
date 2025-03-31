#pragma once
#include <debug-rendering/DebugRenderable.h>
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::pcmodel_editor {
    class PointcloudModelEditor : public OperationMode<Context> {
    public:
        PointcloudModelEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host);

        virtual void ProcessAction(const ActionBase& action) override;
        virtual void RenderScene() override;
    };
}
