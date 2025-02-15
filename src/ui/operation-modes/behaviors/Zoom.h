#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include <utilities/math/MathUtils.h>
#include <utilities/NameHash.h>
#include "ForwardDeclarations.h"

class ZoomBehavior : public OperationModeBehavior {
public:
	float zoomLevel{ 1.0f };
	float sensitivity{ 0.25f };

	static constexpr unsigned int id = name_hash("Zoom");
	virtual unsigned int GetId() override { return id; }

	using OperationModeBehavior::OperationModeBehavior;

	virtual void Render() override {
		if (operationMode.IsMouseOverSceneWindow() && (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))) {
			//if (ImGui::IsKeyPressed(ImGuiKey_MouseWheelY))
				zoomLevel = std::max(0.1f, zoomLevel * std::exp2f(ImGui::GetIO().MouseWheel * sensitivity));
		}
	}
};
