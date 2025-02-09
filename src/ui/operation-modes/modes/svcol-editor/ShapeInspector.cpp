#include "ShapeInspector.h"
#include "Behaviors.h"
#include <ui/common/editors/SvCol.h>

namespace ui::operation_modes::modes::svcol_editor {
	void ShapeInspector::RenderPanel()
	{
		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();
		if (selection.size() == 0)
			ImGui::Text("Select a shape in the left pane.");
		else if (selection.size() > 1)
			ImGui::Text("Multiple shapes selected");
		else
			if (Editor("Shape", *selection[0], GetContext().resource))
				Dispatch(SelectionTransformationBehavior<Context>::SelectionTransformChangedAction{});
	}

	PanelTraits ShapeInspector::GetPanelTraits() const
	{
		return { "Shape inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImVec2(600, ImGui::GetMainViewport()->WorkSize.y - 140), ImVec2(1, 0) };
	}
}
