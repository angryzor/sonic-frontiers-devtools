#include "GameUpdaterInspector.h"

GameUpdaterInspector::GameUpdaterInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator }
{
	SetTitle("GameUpdater");
}

void GameUpdaterInspector::PreRender()
{
	ImGui::SetNextWindowSize(ImVec2(800, 920), ImGuiCond_Once);
}

void GameUpdaterInspector::RenderContents()
{
	auto& gameUpdater = hh::game::GameApplication::GetInstance()->GetGameUpdater();
	auto* gameManager = hh::game::GameManager::GetInstance();

	ImGui::DragFloat("FPS", (*reinterpret_cast<float**>(0x143D907A0) + 5));
	ImGui::DragFloat("Timescale", &gameUpdater.timeScale, 0.01f);

	ImGui::SeparatorText("Per-layer information");
	if (ImGui::BeginTable("Layer information", 10, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX)) {
		ImGui::TableSetupColumn("Layer id");
		ImGui::TableSetupColumn("Normal operation");
		ImGui::TableSetupColumn("Ingame pause");
		ImGui::TableSetupColumn("Debug pause");
		ImGui::TableSetupColumn("Object pause");
		ImGui::TableSetupColumn("Timescale");
		ImGui::TableSetupColumn("DeltaTime");
		ImGui::TableSetupColumn("Current frame");
		ImGui::TableSetupColumn("Unk2");
		ImGui::TableSetupColumn("Unk3");
		ImGui::TableHeadersRow();

		for (size_t i = 0; i < 32; i++) {
			char name[20];
			snprintf(name, sizeof(name), "Layer %zd", i);

			ImGui::PushID(i);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%zd", i);
			ImGui::TableNextColumn();
			ImGui::CheckboxFlags("##layersActiveDuringNormalOperation", &gameUpdater.layersActiveDuringNormalOperation, 1 << i);
			ImGui::TableNextColumn();
			ImGui::CheckboxFlags("##layersActiveDuringIngamePause", &gameUpdater.layersActiveDuringIngamePause, 1 << i);
			ImGui::TableNextColumn();
			ImGui::CheckboxFlags("##layersActiveDuringDebugPause", &gameUpdater.layersActiveDuringDebugPause, 1 << i);
			ImGui::TableNextColumn();
			ImGui::CheckboxFlags("##layersActiveDuringObjectPause", &gameUpdater.layersActiveDuringObjectPause, 1 << i);
			ImGui::TableNextColumn();
			ImGui::DragFloat("##layerTimeScale", &gameUpdater.layerTimeScale[i], 0.01f);
			ImGui::TableNextColumn();
			ImGui::Text("%f", gameUpdater.updateInfos[i].deltaTime);
			ImGui::TableNextColumn();
			ImGui::Text("%d", gameUpdater.updateInfos[i].currentFrame);
			ImGui::TableNextColumn();
			ImGui::Text("%d", gameUpdater.updateInfos[i].unk2);
			ImGui::TableNextColumn();
			ImGui::Text("%x", gameUpdater.updateInfos[i].unk3);
			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}
