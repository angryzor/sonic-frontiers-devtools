#pragma once
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::fxcol_editor {
    class FxColEditor : public OperationMode<Context>
    {
    public:
        FxColEditor(csl::fnd::IAllocator* allocator);
    };
}
