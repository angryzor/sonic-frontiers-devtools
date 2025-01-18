#include "ShapeInspector.h"
#include "Behaviors.h"
#include <ui/common/editors/FxCol.h>

namespace ui::operation_modes::modes::fxcol_editor {
	void ShapeInspector::RenderPanel()
	{
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxColManager>();

		if (!fxColManager)
			return;

		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();
		if (selection.size() == 0) {
			ImGui::Text("Select a shape in the left pane.");
		}
		else if (selection.size() > 1) {
			ImGui::Text("Multiple shapes selected");
		}
		else {
			Editor("Shape", *selection[0], fxColManager->resource);
		}
	}

	PanelTraits ShapeInspector::GetPanelTraits() const
	{
		return { "Shape inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImVec2(600, ImGui::GetMainViewport()->WorkSize.y - 140), ImVec2(1, 0) };
	}
}
