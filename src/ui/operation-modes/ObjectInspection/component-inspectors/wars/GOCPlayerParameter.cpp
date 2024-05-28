#include "GOCPlayerParameter.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Reflection.h>
//#include <ui/resources/editors/ResReflectionEditor.h>

template<typename R>
void RenderModeParameterInspector(const char* label, R (&parameters)[4]) {
    if (ImGui::TreeNode(label)) {
        if (ImGui::TreeNode("Forward view")) {
            Editor("Parameters", *parameters[0]);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Side view")) {
            Editor("Parameters", *parameters[1]);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Forward view water")) {
            Editor("Parameters", *parameters[2]);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Side view water")) {
            Editor("Parameters", *parameters[3]);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
}

const char* playerParameterModes[] = { "Normal", "Water", "Cyberspace Forward View", "Cyberspace Side View" };
const char* playerParameterSuperStates[] = { "Normal", "Super Sonic", "Super Sonic 2" };

void RenderComponentInspector(app::player::GOCPlayerParameter& component) {
	auto* obj = component.owner;

	ComboEnum("Mode", component.mode, playerParameterModes);

	//if (ImGui::Button("Edit character parameters..."))
	//	switch (component.characterId) {
	//	case app::player::CharacterId::SONIC:
	//		ResReflectionEditor::Create(GetAllocator(), &component.characterParameters->sonic);
	//		break;
	//	case app::player::CharacterId::AMY:
	//		ResReflectionEditor::Create(GetAllocator(), &component.characterParameters->amy);
	//		break;
	//	case app::player::CharacterId::KNUCKLES:
	//		ResReflectionEditor::Create(GetAllocator(), &component.characterParameters->knuckles);
	//		break;
	//	case app::player::CharacterId::TAILS:
	//		ResReflectionEditor::Create(GetAllocator(), &component.characterParameters->tails);
	//		break;
	//	}

	//if (ImGui::Button("Edit Camera Set parameters..."))
	//	ResReflectionEditor::Create(GetAllocator(), &*component.cameraSetParameters);

	if (ImGui::TreeNode("Currently loaded parameters")) {
		ImGui::SeparatorText("Parameters");
		if (component.playerCommonParameters) Editor("Common parameters", *component.playerCommonParameters);
		if (component.playerSonicParameters) Editor("Sonic parameters", *component.playerSonicParameters);
		if (component.playerBuddyParameters) Editor("Buddy parameters", *component.playerBuddyParameters);
		if (component.playerSonicClassicParameters) Editor("Classic Sonic parameters", *component.playerSonicClassicParameters);
		if (component.playerSuperSonicParameters) Editor("Super Sonic parameters", *component.playerSuperSonicParameters);
		if (component.gadgetParameters) Editor("Gadget parameters", *component.gadgetParameters);

		ImGui::SeparatorText("Mode packages");
		RenderModeParameterInspector("Common mode packages", component.commonModePackage);
		RenderModeParameterInspector("Sonic mode packages", component.sonicModePackage);
		RenderModeParameterInspector("Buddy mode packages", component.buddyModePackage);
		RenderModeParameterInspector("Classic Sonic mode packages", component.sonicClassicModePackage);

		ImGui::TreePop();
	}
}
