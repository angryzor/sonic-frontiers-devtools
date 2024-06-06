#include "GOCPlayerKinematicParams.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(app::player::GOCPlayerKinematicParams& component)
{
	Editor("Position", component.position);
	Editor("Rotation", component.rotation);
	Editor("Scale", component.scale);
	ImGui::PushID("WorldPos");
	ImGui::SeparatorText("World position");
	ImGui::PopID();
	Editor("World position", component.worldPosition);
	ImGui::SeparatorText("Physics");
	Editor("Velocity", component.velocity);
	Editor("Unk1", component.unk1);
	Editor("Unk2", component.unk2);
	Editor("Unk3", component.unk3);
	Editor("Unk4", component.unk4);
	Editor("Unk5", component.unk5);
	Editor("Unk6", component.unk6);
	ImGui::Text("Unk7");
	Viewer("Unk7", component.unk7);
}
