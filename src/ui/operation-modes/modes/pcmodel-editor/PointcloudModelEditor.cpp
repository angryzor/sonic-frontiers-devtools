#include "PointcloudModelEditor.h"
#include "InstanceList.h"
#include "InstanceInspector.h"
#include "Behaviors.h"
#include <debug-rendering/DebugRenderable.h>
#include <span>

namespace ui::operation_modes::modes::pcmodel_editor {
	using namespace ucsl::resources::pointcloud::v2;

	PointcloudModelEditor::PointcloudModelEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host) : OperationMode{ allocator, host } {
		AddPanel<InstanceList>();
		AddPanel<InstanceInspector>();
		AddBehavior<SelectionBehavior>();
		AddBehavior<SelectionAabbBehavior>();
		AddBehavior<SelectionTransformationBehavior>();
		AddBehavior<SelectionVisual3DBehavior>();
		AddBehavior<ObjectLocationVisual3DBehavior>();
		AddBehavior<GizmoBehavior>();
		AddBehavior<MousePickingBehavior>();
		AddBehavior<SelectionMousePickingBehavior>();
		AddBehavior<PlacementBehavior>();
		AddBehavior<DeleteBehavior>();
		AddBehavior<GroundContextMenuBehavior>();
	}

	void PointcloudModelEditor::ProcessAction(const ActionBase& action) {
		OperationMode::ProcessAction(action);

		switch (action.id) {
		//case SelectionTransformationBehavior<Context>::SelectionTransformChangedAction::id: {
		//	auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

		//	for (auto* instance : selection) {

		//	}

		//	break;
		//}
		}
	}

	void PointcloudModelEditor::RenderScene() {
	}
}
