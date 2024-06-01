#include "StageInfo.h"
#include <ui/common/editors/LevelInfo.h>

void RenderGameServiceInspector(app::level::StageInfo& service)
{
	ImGui::SeparatorText("Stages");
	for (auto& stage : service.stages) {
		if (ImGui::TreeNodeEx(&stage, ImGuiTreeNodeFlags_None, "%s", stage.GetName())) {
			Editor("Stage data", stage);
			ImGui::TreePop();
		}
	}
}
