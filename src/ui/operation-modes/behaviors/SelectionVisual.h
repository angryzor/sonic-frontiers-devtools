#pragma once
#include <debug-rendering/DebugRenderable.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include <utilities/math/MathUtils.h>
#include "ForwardDeclarations.h"

class SelectionVisualBehavior : public OperationModeBehavior {
public:
	static constexpr unsigned int id = 8;
	virtual unsigned int GetId() override { return id; }

	using OperationModeBehavior::OperationModeBehavior;
};

class SelectionVisual2DBehavior : public SelectionVisualBehavior {
public:
	using SelectionVisualBehavior::SelectionVisualBehavior;

	virtual void Render() override {
		auto* selectionAabbBehavior = operationMode.GetBehavior<SelectionAabbBehaviorBase>();

		if (!selectionAabbBehavior || !selectionAabbBehavior->HaveAabb())
			return;

		if (ImGui::Begin("Overlay")) {
			if (auto* dl = ImGui::GetWindowDrawList()) {
				auto selectionAabb = selectionAabbBehavior->GetAabb();

				auto min = NDCCoordsToImGui({ selectionAabb.min.x(), selectionAabb.max.y() });
				auto max = NDCCoordsToImGui({ selectionAabb.max.x(), selectionAabb.min.y() });

				dl->AddRect({ min.x, max.y }, { max.x, min.y }, 0xFFFFFFFF);
			}
		}
		ImGui::End();
	}
};

class SelectionVisual3DBehavior : public SelectionVisualBehavior, public devtools::debug_rendering::DebugRenderable {
public:
	using SelectionVisualBehavior::SelectionVisualBehavior;

	virtual void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) override {
		auto* selectionAabbBehavior = operationMode.GetBehavior<SelectionAabbBehaviorBase>();

		if (!selectionAabbBehavior || !selectionAabbBehavior->HaveAabb())
			return;

		csl::geom::Aabb aabb{ selectionAabbBehavior->GetAabb().min, selectionAabbBehavior->GetAabb().max };
		ctx.DrawAABB(aabb.min, aabb.max, { 255, 255, 255, 140 });
	}
};
