void RenderGameServiceInspector(app::game::GameModeResourceManager& service)
{
	for (auto& module : service.modules) {
		if (ImGui::TreeNodeEx(module, ImGuiTreeNodeFlags_DefaultOpen, "%x", module->GetNameHash())) {
			ImGui::Text("Module flags: %x", module->flags);
			if (ImGui::TreeNode("Levels")) {
				for (auto& level : module->levelLoader->levelsToLoad) {
					ImGui::Text("%s", level.name.c_str());
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}
