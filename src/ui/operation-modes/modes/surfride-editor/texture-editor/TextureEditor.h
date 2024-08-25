#pragma once
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
	class TextureEditor : public OperationMode<Context> {
	public:
		TextureEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host);
		virtual void RenderScene() override;
	};
}
