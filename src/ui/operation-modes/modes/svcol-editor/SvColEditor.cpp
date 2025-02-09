#include "SvColEditor.h"
#include "ShapeList.h"
#include "ShapeInspector.h"
#include "Behaviors.h"
#include <debug-rendering/DebugRenderable.h>
#include <debug-rendering/visuals/SvColShape.h>
//#include <debug-rendering/visuals/KdTree.h>
#include <span>

namespace ui::operation_modes::modes::svcol_editor {
	SvColEditor::SvColEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host) : OperationMode{ allocator, host } {
		AddPanel<ShapeList>();
		AddPanel<ShapeInspector>();
		AddBehavior<SelectionBehavior>();
		AddBehavior<SelectionAabbBehavior>();
		AddBehavior<SelectionTransformationBehavior>();
		AddBehavior<GizmoBehavior>();
		AddBehavior<MousePickingBehavior>();
		AddBehavior<SelectionMousePickingBehavior>();
		AddBehavior<PlacementBehavior>();
		AddBehavior<DeleteBehavior>();
		AddBehavior<GroundContextMenuBehavior>();
	}

	void SvColEditor::ProcessAction(const ActionBase& action) {
		OperationMode::ProcessAction(action);

		switch (action.id) {
		case SelectionTransformationBehavior<Context>::SelectionTransformChangedAction::id: {
			auto& context = GetContext();
			auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

			for (auto* shape : selection)
				context.RecalculateBoundingVolume(*shape);

			break;
		}
		}
	}

	void SvColEditor::RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) {
		auto& context = GetContext();

		if (context.resource == nullptr)
			return;

		//if (renderKdTree)
		//	RenderDebugVisual(ctx, context.resource->svColData.);

		auto& svColData = *context.resource->svColData;

		if (renderBoundingVolumes)
			for (auto& shape : std::span(svColData.shapes, svColData.shapeCount))
				ctx.DrawAABB(shape.aabbMin, shape.aabbMax, { 0, 0, 255, 255 });

		if (renderCollisionShapes)
			for (auto& shape : std::span(svColData.shapes, svColData.shapeCount))
				RenderDebugVisual(ctx, shape);
	}

	void SvColEditor::RenderScene() {
		if (ImGui::Begin("Main menu")) {
			ImGui::SameLine();
			ImGui::Checkbox("Render bounding volumes", &renderBoundingVolumes);
			ImGui::SameLine();
			ImGui::Checkbox("Render collision shapes", &renderCollisionShapes);
			//ImGui::SameLine();
			//ImGui::Checkbox("Render bounding volumes K/D tree", &renderKdTree);
		}
		ImGui::End();
	}
}
