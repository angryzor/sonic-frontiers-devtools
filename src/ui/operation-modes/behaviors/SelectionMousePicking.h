#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include "ForwardDeclarations.h"
#include "Selection.h"
#include "MousePicking.h"

template<typename OpModeContext>
class SelectionMousePickingBehavior : public OperationModeBehavior {
public:
	static constexpr unsigned int id = 9;
	virtual unsigned int GetId() override { return id; }

	using OperationModeBehavior::OperationModeBehavior;

	virtual void Render() override {
		auto* selectionBehavior = operationMode.GetBehavior<SelectionBehavior<OpModeContext>>();
		auto* mousePicking = operationMode.GetBehavior<MousePickingBehavior<OpModeContext>>();

		if (mousePicking->picked && mousePicking->mouseButton == ImGuiMouseButton_Left) {
			if (ImGui::IsKeyDown(ImGuiKey_ModCtrl))
				selectionBehavior->ToggleSelection(mousePicking->pickedObjects);
			else
				selectionBehavior->Select(mousePicking->pickedObjects);
		}
	}
};
