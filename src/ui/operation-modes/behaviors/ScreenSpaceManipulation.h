#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include <utilities/math/MathUtils.h>
#include "SelectionTransformation.h"
#include "ForwardDeclarations.h"

class ScreenSpaceManipulationBehavior : public OperationModeBehavior {
	bool dragging{ false };

public:
	static constexpr unsigned int id = 11;
	virtual unsigned int GetId() override { return id; }

	using OperationModeBehavior::OperationModeBehavior;

	bool IsMouseInSelectionAabb()
	{
		auto selAabb = operationMode.GetBehavior<SelectionAabbBehaviorBase>();

		if (!selAabb->HaveAabb())
			return false;

		auto selectionAabb = selAabb->GetAabb();

		auto& io = ImGui::GetIO();
		auto mousePos = ImGui::GetMousePos();

		auto min = NDCCoordsToImGui({ selectionAabb.min.x(), selectionAabb.max.y() });
		auto max = NDCCoordsToImGui({ selectionAabb.max.x(), selectionAabb.min.y() });

		return min.x <= mousePos.x && min.y <= mousePos.y && max.x >= mousePos.x && max.y >= mousePos.y;
	}

	virtual void Render() {
		auto* selTransform = operationMode.GetBehavior<SelectionTransformationBehaviorBase<true>>();

		if (!selTransform->HasTransformableSelection())
			return;

		if (!operationMode.CanTakeMouseControl(this))
			return;

		operationMode.ToggleDragging(this, IsMouseInSelectionAabb());

		if (!operationMode.IsDragging(this))
			return;

		auto& io = ImGui::GetIO();
		auto delta = io.MousePos - io.MousePosPrev;
		selTransform->SetSelectionTransform(Eigen::Translation3f{ Eigen::Vector3f{ 2.0f * delta.x / io.DisplaySize.x, 2.0f * -delta.y / io.DisplaySize.y, 0.0f } } * selTransform->GetSelectionTransform());
	}
};
