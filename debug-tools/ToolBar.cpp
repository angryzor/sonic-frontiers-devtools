#include "Pch.h"
#include "ToolBar.h"

void ToolBar::Render() {
	ImGuiWindowFlags flags
		= ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoCollapse;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 0));

	if (!ImGui::Begin("Sonic Frontiers Devtools", NULL, flags)) {
		ImGui::End();
		return;
	}

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Game")) {
			ImGui::MenuItem("Switch GameMode...");
			ImGui::MenuItem("Load Level...");
			ImGui::EndMenu();
		}
		ImGui::MenuItem("Open Library");
		if (ImGui::BeginMenu("Mode")) {
			ImGui::MenuItem("Object Inspection");
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}
