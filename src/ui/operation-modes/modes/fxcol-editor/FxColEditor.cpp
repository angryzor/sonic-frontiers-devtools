#include "FxColEditor.h"
#include "ShapeList.h"
#include "ShapeInspector.h"
#include "Behaviors.h"

namespace ui::operation_modes::modes::fxcol_editor {
	FxColEditor::FxColEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host) : OperationMode{ allocator, host } {
		AddPanel<ShapeList>();
		AddPanel<ShapeInspector>();
		AddBehavior<SelectionBehavior>();
		AddBehavior<SelectionAabbBehavior>();
		AddBehavior<SelectionTransformationBehavior>();
		AddBehavior<SelectionVisual3DBehavior>();
		AddBehavior<GizmoBehavior>();
		AddBehavior<MousePickingBehavior>();
		AddBehavior<SelectionMousePickingBehavior>();
		AddBehavior<RenderFxColBehavior>();
		AddBehavior<PlacementBehavior>();
		AddBehavior<DeleteBehavior>();
		AddBehavior<GroundContextMenuBehavior>();
	}
}
