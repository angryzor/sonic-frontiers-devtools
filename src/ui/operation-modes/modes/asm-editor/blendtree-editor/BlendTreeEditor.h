#pragma once
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::asm_editor::blend_tree_editor {
	class BlendTreeEditor : public OperationMode<Context> {
	public:
		BlendTreeEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host);
		virtual void RenderScene() override;
	};
}
