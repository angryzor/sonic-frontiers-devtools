#include "Pch.h"
#include "ToolBar.h"
#include "Theme.h"
#include "Desktop.h"

void ToolBar::Render(Desktop* desktop) {
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

		bool openResourceBrowser{ false };
		ImGui::MenuItem("Resource Browser", nullptr, &openResourceBrowser);

		if (openResourceBrowser)
			desktop->OpenResourceBrowser();

		if (ImGui::BeginMenu("Mode")) {
			ImGui::MenuItem("Object Inspection");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Theme")) {
			for (size_t i = 0; i < 3; i++) {
				bool selected{ false };

				ImGui::MenuItem(Theme::themes[i].name, nullptr, &selected);

				if (selected) {
					Theme::themes[i].Load();
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}
