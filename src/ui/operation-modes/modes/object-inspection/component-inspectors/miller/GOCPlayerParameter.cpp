#include "GOCPlayerParameter.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Reflection.h>
#include <ui/Desktop.h>
#include <ui/resources/editors/ResReflectionEditor.h>

template<typename R>
void RenderModeParameterInspector(const char* label, R(&parameters)[4]) {
	if (ImGui::TreeNode(label)) {
		if (ImGui::TreeNode("Forward View")) {
			Editor("Parameters", *parameters[0]);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Side View")) {
			Editor("Parameters", *parameters[1]);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Boss")) {
			Editor("Parameters", *parameters[2]);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("White Space")) {
			Editor("Parameters", *parameters[3]);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

const char* playerParameterModes[] = { "Forward View", "Side View", "Boss", "White Space" };

void RenderComponentInspector(app::player::GOCPlayerParameter& component) {
	ComboEnum("Mode", component.mode, playerParameterModes);

	if (ImGui::Button("Edit Player parameters..."))
		ResReflectionEditor::Create(Desktop::instance->GetAllocator(), &*component.playerParameters);

	if (ImGui::Button("Edit Camera Set parameters..."))
		ResReflectionEditor::Create(Desktop::instance->GetAllocator(), &*component.cameraSetParameters);

	if (ImGui::TreeNode("Currently loaded mode parameters")) {
		ImGui::SeparatorText("Mode packages");
		RenderModeParameterInspector("Mode packages", component.modePackages);

		ImGui::SeparatorText("Parameters");
		RenderModeParameterInspector("common", component.commonParameters);
		RenderModeParameterInspector("speed", component.speedParameters);
		RenderModeParameterInspector("jump", component.jumpParameters);
		RenderModeParameterInspector("jumpSpeed", component.jumpSpeedParameters);
		RenderModeParameterInspector("doubleJump", component.doubleJumpParameters);
		RenderModeParameterInspector("boost", component.boostParameters);
		RenderModeParameterInspector("airBoost", component.airBoostParameters);
		ImGui::TreePop();
	}
}
