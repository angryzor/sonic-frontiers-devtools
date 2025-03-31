#include "LevelManager.h"
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>

void RenderGameServiceInspector(hh::game::LevelManager& levelManager) {
	ImGui::SeparatorText("Master levels");
	for (auto* mlevel : levelManager.masterLevels) {
		if (ImGui::TreeNode(mlevel->resource->GetName())) {
			for (auto* level : mlevel->levels) {
				if (ImGui::TreeNode(level->name.c_str())) {
					Viewer("Initialized", level->loaded);
					Viewer("Load status", level->loadStatus);
					Viewer("byteD0", level->byteD0);
					Viewer("dwordD4", level->dwordD4);
					Viewer("unk1", level->unk1);
					Viewer("unk2", level->unk2);

					if (ImGui::Button("Load"))
						level->Load3({ .unk2 = -2 });

					ImGui::SameLine();

					if (ImGui::Button("Unload"))
						level->Unload();
					
					ImGui::SeparatorText("Dependencies");
					for (auto* dep : level->dependencies)
						ImGui::Text("%s", dep->name.c_str());

					ImGui::SeparatorText("Resources");
					for (auto& resource : level->files)
						ImGui::Text("%s", resource.c_str());
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
}
