#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include <utilities/math/MathUtils.h>
#include "SelectionTransformation.h"
#include "Zoom.h"
#include "ForwardDeclarations.h"

template<typename OpModeContext>
class ScreenSpaceManipulationBehavior : public OperationModeBehavior {
	using Traits = ScreenSpaceManipulationBehaviorTraits<OpModeContext>;

	Traits traits;
	bool dragging{ false };

public:
	static constexpr unsigned int id = 11;
	virtual unsigned int GetId() override { return id; }

	ScreenSpaceManipulationBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode)
		: OperationModeBehavior{ allocator, operationMode }, traits{ operationMode.GetContext() } {}

	bool IsMouseInSelectionAabb()
	{
		auto selAabb = operationMode.GetBehavior<SelectionAabbBehaviorBase>();
		auto* zoomBehavior = operationMode.GetBehavior<ZoomBehavior>();
		auto zoom = zoomBehavior == nullptr ? 1.0f : zoomBehavior->zoomLevel;

		if (!selAabb->HaveAabb())
			return false;

		auto selectionAabb = selAabb->GetAabb();

		auto mousePos = traits.ImGuiToSelectionSpace((ImGui::GetMousePos() - ImGui::GetWindowContentRegionMin() - ImGui::GetWindowPos()) / zoom);

		return selectionAabb.min.x() <= mousePos.x() && selectionAabb.min.y() <= mousePos.y() && selectionAabb.max.x() >= mousePos.x() && selectionAabb.max.y() >= mousePos.y();
	}

	virtual void Render() {
		auto* selTransform = operationMode.GetBehavior<SelectionTransformationBehaviorBase<true>>();
		auto* zoomBehavior = operationMode.GetBehavior<ZoomBehavior>();
		auto zoom = zoomBehavior == nullptr ? 1.0f : zoomBehavior->zoomLevel;

		if (!selTransform->HasTransformableSelection())
			return;

		if (!operationMode.CanTakeMouseControl(this))
			return;

		operationMode.ToggleDragging(this, IsMouseInSelectionAabb());

		if (!operationMode.IsDragging(this))
			return;

		auto& io = ImGui::GetIO();
		selTransform->SetSelectionTransform(Eigen::Translation3f{ traits.ImGuiToSelectionSpace(io.MousePos / zoom) - traits.ImGuiToSelectionSpace(io.MousePosPrev / zoom) } *selTransform->GetSelectionTransform());
	}
};
