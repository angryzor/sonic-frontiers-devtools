#include "ShapeList.h"
#include "Behaviors.h"

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

		for (size_t i = 0; i < fxColData->boundingVolumeCount; i++) {
			auto& boundingVolume = fxColData->boundingVolumes[i];
			char name[100];
			snprintf(name, sizeof(name), "Bounding Volume %zd", i);

			if (ImGui::TreeNode(&boundingVolume, name)) {
				for (size_t j = 0; j < boundingVolume.shapeCount; j++)
					RenderShapeItem(fxColData->collisionShapes[boundingVolume.shapeStartIdx + j]);

				ImGui::TreePop();
			}
		}

		for (size_t i = 0; i < fxColData->collisionShapeCount; i++) {
			bool found{};

			for (size_t j = 0; j < fxColData->boundingVolumeCount; j++) {
				if (i >= fxColData->boundingVolumes[j].shapeStartIdx && i < fxColData->boundingVolumes[j].shapeStartIdx + fxColData->boundingVolumes[j].shapeCount) {
					found = true;
					break;
				}
			}

			if (found)
				break;

			RenderShapeItem(fxColData->collisionShapes[i]);
		}
	}

	PanelTraits ShapeList::GetPanelTraits() const {
		return PanelTraits{ "Shape List", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 100) };
	}

	void ShapeList::RenderShapeItem(FxColCollisionShape& shape)
	{
		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();

		if (ImGui::Selectable(shape.name, selectionBehavior->GetSelection().find(&shape) != -1))
			selectionBehavior->Select(&shape);
	}
}
