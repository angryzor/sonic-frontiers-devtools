#include "GOCAnimator.h"

void RenderBlendNode(hh::anim::BlendNodeBase* node) {
	const char* type;

	switch (node->type) {
	case hh::anim::BlendNodeType::LERP:
		type = "LERP";
		break;
	case hh::anim::BlendNodeType::ADDITIVE:
		type = "ADDITIVE";
		break;
	case hh::anim::BlendNodeType::CLIP:
		type = "CLIP";
		break;
	case hh::anim::BlendNodeType::OVERRIDE:
		type = "OVERRIDE";
		break;
	case hh::anim::BlendNodeType::LAYER:
		type = "LAYER";
		break;
	case hh::anim::BlendNodeType::MULTIPLY:
		type = "MULTIPLY";
		break;
	case hh::anim::BlendNodeType::BLEND_SPACE:
		type = "BLENDSPACE";
		break;
	case hh::anim::BlendNodeType::TWO_POINT_LERP:
		type = "TWO POINT LERP";
		break;
	default:
		type = "UNKNOWN";
		break;
	}

	if (ImGui::TreeNodeEx(node, ImGuiTreeNodeFlags_DefaultOpen, "%x - type %s - flags %x", node, type, node->flags)) {
		for (auto* child : node->children)
			RenderBlendNode(child);
		ImGui::TreePop();
	}
}

void RenderComponentInspector(hh::anim::GOCAnimator& component)
{
	bool nope{ false };
	const char* select{ "Select one" };
	if (ImGui::BeginCombo("Current state", select)) {
		auto& stateIds = component.asmResourceManager->animatorResource->stateIdsByName;
		for (auto it = stateIds.begin(); it != stateIds.end(); it++) {
			if (ImGui::Selectable(it.key(), &nope)) {
				component.ChangeState(it.key());
			}
		}
		ImGui::EndCombo();
	}
	//if (ImGui::BeginCombo("Variables", select)) {
	ImGui::SeparatorText("Variables");
	auto& variableIds = component.asmResourceManager->animatorResource->variableIdsByName;

	for (auto it = variableIds.begin(); it != variableIds.end(); it++) {
		ImGui::Text("%d: %s", *it, it.key());
	}
	//}

	if (component.animationStateMachine->blendTree) {
		ImGui::SeparatorText("BlendNodes");
		RenderBlendNode(component.animationStateMachine->blendTree);
	}

	ImGui::SeparatorText("Layers");
	for (auto& layer : component.animationStateMachine->layers) {
		if (ImGui::TreeNodeEx(&layer, ImGuiTreeNodeFlags_None, "%d - %s", layer.layerId, component.asmResourceManager->animatorResource->binaryData->layers[layer.layerId].name)) {
			ImGui::Text("%d", layer.layerId);
			ImGui::Text("%x", layer.unk2);
			ImGui::Text("%x", layer.unk3);
			ImGui::Text("%x", layer.unk4);
			ImGui::Text("%x", layer.unk5);
			ImGui::Text("%x", layer.unk6);
			ImGui::Text("%x", layer.unk7);
			ImGui::Text("%x", layer.unk8);
			RenderBlendNode(layer.blendNode);
			ImGui::TreePop();
		}
	}
}
