#include "SurfRideEditor.h"
#include "ProjectTree.h"
#include "Timeline.h"
#include "ElementInspector.h"
#include "Behaviors.h"
#include <ui/common/editors/Reflection.h>
#include <reflection/serialization/resource-rfls/ResourceRfls.h>

namespace ui::operation_modes::modes::surfride_editor
{
	SurfRideEditor::SurfRideEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host) : OperationMode{ allocator, host }
	{
		AddPanel<ProjectTree>();
		AddPanel<Timeline>();
		AddPanel<ElementInspector>();
		AddBehavior<SelectionBehavior>();
		AddBehavior<SelectionAabbBehavior>();
		AddBehavior<SelectionTransformationBehavior>();
		AddBehavior<SelectionVisual2DBehavior>();
		AddBehavior<ScreenSpaceManipulationBehavior>();
		AddBehavior<MousePickingBehavior>();
		AddBehavior<SelectionMousePickingBehavior>();
	}

	void SurfRideEditor::Render() {
		OperationMode::Render();

		//if (GetContext().gocSprite && GetContext().gocSprite->GetProject())
		//	ReflectionEditor("Raw data", const_cast<SurfRide::SRS_PROJECT*>(GetContext().gocSprite->GetProject()->projectData), &hh::fnd::RflClassTraits<SurfRide::SRS_PROJECT>::rflClass);
	}
}
