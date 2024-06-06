#include "LevelInfo.h"
#include <ui/common/editors/LevelInfo.h>

void RenderGameServiceInspector(app::level::LevelInfo& service)
{
	auto* stageData = service.GetStageData();
	ImGui::SeparatorText("Stage data");
	if (stageData)
		Editor("Stage data", *stageData);
	else
		ImGui::Text("No stage data loaded.");

	if (ImGui::TreeNode("PlayerInformation")) {
		for (size_t i = 0; i < 2; i++) {
			auto* playerInfo = service.GetPlayerInformation(i);
			char nodeName[50];
			snprintf(nodeName, sizeof(nodeName), "Player %zd", i);

			if (ImGui::TreeNode(nodeName)) {
				Editor(nodeName, *playerInfo);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}
