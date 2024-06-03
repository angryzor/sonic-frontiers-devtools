#include "StageInfo.h"
#include <ui/common/editors/LevelInfo.h>

void RenderGameServiceInspector(app::level::StageInfo& service)
{
	ImGui::SeparatorText("Stages");
#ifdef DEVTOOLS_TARGET_SDK_wars
	for (auto& stage : service.stages) {
#else
	for (auto& stage_ : service.stages) {
		auto& stage = *stage_;
#endif
		if (ImGui::TreeNodeEx(&stage, ImGuiTreeNodeFlags_None, "%s", stage.GetName())) {
			Editor("Stage data", stage);
			ImGui::TreePop();
		}
	}
}
