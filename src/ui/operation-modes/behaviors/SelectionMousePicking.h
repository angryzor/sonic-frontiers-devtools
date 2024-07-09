#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include "Selection.h"
#include "MousePicking.h"

template<typename T>
class SelectionMousePickingBehavior : public OperationModeBehavior {
public:
	static constexpr unsigned int id = 9;
	virtual unsigned int GetId() override { return id; }

	using OperationModeBehavior::OperationModeBehavior;

	virtual void Render() override {
		auto* selectionBehavior = operationMode.GetBehavior<SelectionBehavior<T>>();
		auto* mousePicking = operationMode.GetBehavior<MousePickingBehaviorBase<T>>();

		if (mousePicking->picked && mousePicking->mouseButton == ImGuiMouseButton_Left)
			selectionBehavior->Select(mousePicking->pickedObjects);
	}
};
