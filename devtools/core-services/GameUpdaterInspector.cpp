#include "../Pch.h"
#include "GameUpdaterInspector.h"

GameUpdaterInspector::GameUpdaterInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator }
{
	SetTitle("GameUpdater");
}

void GameUpdaterInspector::RenderContents()
{
	auto& gameUpdater = hh::game::GameApplication::GetInstance()->GetGameUpdater();
	//unsigned int flags = static_cast<unsigned int>(gameUpdater.flags.m_dummy);
	//ImGui::CheckboxFlags("Unknown pause", &flags, 1);
	//ImGui::CheckboxFlags("Full pause", &flags, 2);
	//ImGui::CheckboxFlags("flag 2", &flags, 4);
	//ImGui::CheckboxFlags("flag 3", &flags, 8);
	//ImGui::CheckboxFlags("Step single frame", &flags, 16);
	//ImGui::CheckboxFlags("Object pause", &flags, 32);
	//ImGui::CheckboxFlags("flag 6", &flags, 64);
	//ImGui::CheckboxFlags("flag 7", &flags, 128);
	//gameUpdater.flags.m_dummy = static_cast<hh::game::GameUpdater::Flags>(flags);

	//ImGui::Text("unk7.unk1.unk1: %x", gameUpdater.unk7.unk1.unk1);
	//ImGui::Text("unk7.unk1.unk2: %x", gameUpdater.unk7.unk1.unk2);
	//ImGui::Text("unk7.unk2.unk1: %x", gameUpdater.unk7.unk2.unk1);
	//ImGui::Text("unk7.unk2.unk2: %x", gameUpdater.unk7.unk2.unk2);

	ImGui::Text("layers active during normal operation: %x", gameUpdater.layersActiveDuringNormalOperation);
	ImGui::Text("layers active during ingame pause: %x", gameUpdater.layersActiveDuringIngamePause);
	ImGui::Text("layers active during debug pause: %x", gameUpdater.layersActiveDuringDebugPause);
	ImGui::Text("layers active during object pause: %x", gameUpdater.layersActiveDuringObjectPause);

	//ImGui::DragFloat("unkFloat5", &gameUpdater.unk5);
	//ImGui::DragFloat("unkFloat6", &gameUpdater.unk6);

	//for (int i = 0; i < 32; i++) {
	//	char name[100];
	//	snprintf(name, sizeof(name), "unk float for layer %d", i);
	//	ImGui::DragFloat(name, &gameUpdater.maybeFrameTimes[i]);
	//}

	for (int i = 0; i < 32; i++) {
		ImGui::Text("updateInfo for layer %d: deltaTime (s): %f, total time (ms): %d, unk2: %x, unk3: %x", i, gameUpdater.unk8[i].deltaTime, gameUpdater.unk8[i].currentFrame, gameUpdater.unk8[i].unk2, gameUpdater.unk8[i].unk3);
	}

	//float deltaTimes[32];

	//for (int i = 0; i < 32; i++) {
	//	deltaTimes[i] = gameUpdater.unk8[i].deltaTime;
	//}
}
