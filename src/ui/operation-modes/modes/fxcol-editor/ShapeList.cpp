#include "ShapeList.h"
#include <ui/operation-modes/behaviors/Selection.h>

namespace ui::operation_modes::modes::fxcol_editor {
	using namespace app::gfx;

	void ShapeList::RenderPanel() {
		auto* fxColManager = hh::game::GameManager::GetInstance()->GetService<FxColManager>();

		if (!fxColManager) {
			ImGui::Text("FxColManager service not active.");
			return;
		}

		if (!fxColManager->resource) {
			ImGui::Text("No resource loaded.");
			return;
		}

		auto* fxColData = fxColManager->resource->fxColData;
		auto* selectionBehavior = GetBehavior<SelectionBehavior<FxColCollisionShape*>>();

		for (size_t i = 0; i < fxColData->collisionShapeCount; i++) {
			auto* shape = &fxColData->collisionShapes[i];

			if (ImGui::Selectable(shape->ownerName, selectionBehavior->GetSelection().find(shape) != -1))
				GetBehavior<SelectionBehavior<FxColCollisionShape*>>()->Select(shape);
		}
	}

	PanelTraits ShapeList::GetPanelTraits() const {
		return PanelTraits{ "Shape List", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100) };
	}
}
