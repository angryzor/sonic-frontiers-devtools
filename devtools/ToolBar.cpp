#include "Pch.h"
#include "ToolBar.h"
#include "Theme.h"
#include "Desktop.h"
#include "ResourceBrowser.h"
#include "core-services/GameUpdaterInspector.h"

void ToolBar::Render() {
	ImGuiWindowFlags windowFlags
		= ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoCollapse;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 0));

	if (!ImGui::Begin("Sonic Frontiers Devtools", NULL, windowFlags)) {
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
			new (Desktop::instance->GetAllocator()) ResourceBrowser(Desktop::instance->GetAllocator());

		if (ImGui::BeginMenu("Insights")) {
			if (ImGui::MenuItem("GameUpdater"))
				new (Desktop::instance->GetAllocator()) GameUpdaterInspector(Desktop::instance->GetAllocator());
			ImGui::EndMenu();
		}

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

	auto& gameUpdater = (*rangerssdk::bootstrap::GetAddress(&hh::game::GameApplication::instance))->GetGameUpdater();

	unsigned int gameUpdaterFlags = static_cast<unsigned int>(gameUpdater.flags.m_dummy);

	ImGui::CheckboxFlags("Object pause", &gameUpdaterFlags, 1 << static_cast<uint8_t>(hh::game::GameUpdater::Flags::OBJECT_PAUSE));
	ImGui::SameLine();
	ImGui::CheckboxFlags("Debug pause", &gameUpdaterFlags, 1 << static_cast<uint8_t>(hh::game::GameUpdater::Flags::DEBUG_PAUSE));
	ImGui::SameLine();

	gameUpdater.flags.m_dummy = static_cast<hh::game::GameUpdater::Flags>(gameUpdaterFlags);

	if (ImGui::Button("Step frame"))
		gameUpdater.flags.set(hh::game::GameUpdater::Flags::DEBUG_STEP_FRAME);

	if (ImGui::IsKeyPressed(ImGuiKey_F3))
		gameUpdater.flags.flip(hh::game::GameUpdater::Flags::OBJECT_PAUSE);
	if (ImGui::IsKeyPressed(ImGuiKey_F4))
		gameUpdater.flags.flip(hh::game::GameUpdater::Flags::DEBUG_PAUSE);
	if (ImGui::IsKeyPressed(ImGuiKey_F5))
		gameUpdater.flags.set(hh::game::GameUpdater::Flags::DEBUG_STEP_FRAME);

	ImGui::End();
}
