#include "FxColEditor.h"
#include "Behaviors.h"

namespace ui::operation_modes::modes::fxcol_editor {
	FxColEditor::FxColEditor(csl::fnd::IAllocator* allocator) : OperationMode{ allocator } {
		AddPanel<ShapeList>();
		AddPanel<ShapeInspector>();
		AddBehavior<SelectionBehavior>();
		AddBehavior<SelectionAabbBehavior>();
		AddBehavior<SelectionTransformationBehavior>();
		AddBehavior<GizmoBehavior>();
		AddBehavior<MousePickingBehavior>();
		AddBehavior<SelectionMousePickingBehavior>();
		AddBehavior<RenderFxColBehavior>();
	}
}
