#include "NodeEditor.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

namespace ui::operation_modes::modes::asm_editor {
	using namespace hh::anim;
	namespace NodeEd = ax::NodeEditor;

	ImVec4 NodeEditorInterface::GetColor(PinType type)
	{
		switch (type) {
		case PinType::DEFAULT_TRANSITION: return { 1.0f, 1.0f, 1.0f, 1.0f };
		//case PinType::TRANSITION: return { 0.0f, 0.8f, 0.8f, 1.0f };
		case PinType::TRANSITION: return { 0.0f, 0.5f, 0.8f, 1.0f };
		case PinType::EVENT: return { 0.965f, 0.243f, 0.235f, 1.0f };
		case PinType::VARIABLE: return { 0.165f, 0.886f, 0.682f, 1.0f };
		case PinType::BLEND_NODE: return { 0.886f, 0.322f, 0.665f, 1.0f };
		case PinType::BLEND_MASK: return { 0.886f, 0.682f, 0.165f, 1.0f };
		case PinType::CLIP: return { 0.58f, 0.788f, 0.0f, 1.0f };
		case PinType::BLEND_SPACE: return { 0.651f, 0.325f, 0.0f, 1.0f };
		default: assert(false); return { 0.0f, 0.0f, 0.0f, 0.0f }; break;
		}
	}

	void NodeEditorInterface::RenderPinIcon(PinType type) {
		auto size = ImGui::GetTextLineHeight();
		auto color = ImGui::GetColorU32(GetColor(type));

		ImGui::Dummy({ size, size });

		auto itemRectMin = ImGui::GetItemRectMin();
		auto itemRectMax = ImGui::GetItemRectMax();

		switch (type) {
		case PinType::DEFAULT_TRANSITION: {
			ImVec2 arrowHead[] = { { itemRectMin.x, itemRectMin.y }, { itemRectMin.x + size / 2, itemRectMin.y }, { itemRectMin.x + size, itemRectMin.y + size / 2 }, { itemRectMin.x + size / 2, itemRectMin.y + size }, { itemRectMin.x, itemRectMin.y + size } };
			ImGui::GetWindowDrawList()->AddConvexPolyFilled(arrowHead, 5, color);
			break;
		}
		case PinType::TRANSITION: {
			ImVec2 arrowHead[] = { { itemRectMin.x, itemRectMin.y }, { itemRectMin.x + size / 2, itemRectMin.y }, { itemRectMin.x + size, itemRectMin.y + size / 2 }, { itemRectMin.x + size / 2, itemRectMin.y + size }, { itemRectMin.x, itemRectMin.y + size } };
			ImGui::GetWindowDrawList()->AddConvexPolyFilled(arrowHead, 5, color);
			break;
		}
		case PinType::EVENT: {
			ImGui::GetWindowDrawList()->AddRectFilled(itemRectMin, itemRectMax, color);
			break;
		}
		case PinType::VARIABLE: {
			ImGui::GetWindowDrawList()->AddCircleFilled(itemRectMin + ImVec2{ size / 2, size / 2 }, size / 2, color);
			break;
		}
		case PinType::BLEND_NODE: {
			ImGui::GetWindowDrawList()->AddCircleFilled(itemRectMin + ImVec2{ size / 2, size / 2 }, size / 2, color);
			break;
		}
		case PinType::BLEND_MASK: {
			ImGui::GetWindowDrawList()->AddCircleFilled(itemRectMin + ImVec2{ size / 2, size / 2 }, size / 2, color);
			break;
		}
		case PinType::CLIP: {
			ImGui::GetWindowDrawList()->AddCircleFilled(itemRectMin + ImVec2{ size / 2, size / 2 }, size / 2, color);
			break;
		}
		case PinType::BLEND_SPACE: {
			ImGui::GetWindowDrawList()->AddCircleFilled(itemRectMin + ImVec2{ size / 2, size / 2 }, size / 2, color);
			break;
		}
		default: {
			assert(false);
			break;
		}
		}
	}

	NodeEditor::NodeEditor(csl::fnd::IAllocator* allocator, hh::anim::ResAnimator& resource, hh::anim::GOCAnimator* gocAnimator) : CompatibleObject{ allocator }, resource{ resource }, nodeEditor { allocator }, asmData{ *resource.binaryData }, gocAnimator{ gocAnimator } {}

	void NodeEditor::Begin() {
		nodeEditor.Begin();
	}

	void NodeEditor::End() {
		nodeEditor.End();
	}

	void NodeEditor::RunAutoLayout()
	{
		nodeEditor.RunAutoLayout();
	}

	void NodeEditor::State(short stateId, ImVec4 color, float progress) {
		auto& state = asmData.states[stateId];
		auto nodeId = GetNodeId(NodeType::STATE, stateId);

		float maxTextWidth = 0.0f;
		for (unsigned short j = 0; j < state.eventCount; j++) {
			auto& event = asmData.events[state.eventOffset + j];
			maxTextWidth = std::fmaxf(maxTextWidth, ImGui::CalcTextSize(event.name).x);
		}

		nodeEditor.BeginNode(nodeId, maxTextWidth);

		nodeEditor.BeginInputPins();
		InputPin(nodeId, PinType::DEFAULT_TRANSITION, 0);
		InputPin(nodeId, PinType::CLIP, 0);
		BeginInputPin(nodeId, PinType::VARIABLE, 0);
		Editor("Speed", state.speed);
		EndInputPin();
		nodeEditor.EndInputPins();

		nodeEditor.BeginControls();
		ImGui::ProgressBar(progress, { ImGui::CalcTextSize(state.name).x + 10.0f, 0.0f }, state.name);
		Editor("Loop count", state.maxCycles);
		CheckboxFlags("Loop", state.flags, StateData::Flag::LOOPS);
		CheckboxFlags("Unknown 1", state.flags, StateData::Flag::UNK1);
		nodeEditor.EndControls();

		nodeEditor.BeginOutputPins();
		OutputPin(nodeId, PinType::DEFAULT_TRANSITION, 0);
		OutputPin(nodeId, PinType::TRANSITION, 0);
		for (unsigned short j = 0; j < state.eventCount; j++)
			OutputPin(nodeId, PinType::EVENT, j, asmData.events[state.eventOffset + j].name);
		nodeEditor.EndOutputPins();

		nodeEditor.EndNode();
	}

	void NodeEditor::StateTransition(short prevStateId, short nextStateId)
	{
		auto prevNodeId = GetNodeId(NodeType::STATE, prevStateId);
		auto nextNodeId = GetNodeId(NodeType::STATE, nextStateId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::TRANSITION, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::DEFAULT_TRANSITION, 0);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::TRANSITION);
	}

	void NodeEditor::StateDefaultTransition(short prevStateId, short nextStateId)
	{
		auto prevNodeId = GetNodeId(NodeType::STATE, prevStateId);
		auto nextNodeId = GetNodeId(NodeType::STATE, nextStateId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::DEFAULT_TRANSITION, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::DEFAULT_TRANSITION, 0);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::DEFAULT_TRANSITION);
		nodeEditor.LayoutLink(prevNodeId, nextNodeId);
	}

	void NodeEditor::StateEventTransition(short prevStateId, short nextStateId, unsigned short idx)
	{
		auto prevNodeId = GetNodeId(NodeType::STATE, prevStateId);
		auto nextNodeId = GetNodeId(NodeType::STATE, nextStateId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::EVENT, idx);
		auto nextPin = GetInputPinId(nextNodeId, PinType::DEFAULT_TRANSITION, 0);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::EVENT);
		nodeEditor.LayoutLink(prevNodeId, nextNodeId);
	}

	void NodeEditor::StateTransitionFlow(short prevStateId, short nextStateId)
	{
		auto prevNodeId = GetNodeId(NodeType::STATE, prevStateId);
		auto nextNodeId = GetNodeId(NodeType::STATE, nextStateId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::TRANSITION, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::DEFAULT_TRANSITION, 0);

		nodeEditor.Flow(GetLinkId(prevPin, nextPin));
	}

	void NodeEditor::StateDefaultTransitionFlow(short prevStateId, short nextStateId)
	{
		auto prevNodeId = GetNodeId(NodeType::STATE, prevStateId);
		auto nextNodeId = GetNodeId(NodeType::STATE, nextStateId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::DEFAULT_TRANSITION, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::DEFAULT_TRANSITION, 0);

		nodeEditor.Flow(GetLinkId(prevPin, nextPin));
	}

	void NodeEditor::StateEventTransitionFlow(short prevStateId, short nextStateId, unsigned short idx)
	{
		auto prevNodeId = GetNodeId(NodeType::STATE, prevStateId);
		auto nextNodeId = GetNodeId(NodeType::STATE, nextStateId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::EVENT, idx);
		auto nextPin = GetInputPinId(nextNodeId, PinType::DEFAULT_TRANSITION, 0);

		nodeEditor.Flow(GetLinkId(prevPin, nextPin));
	}

	void NodeEditor::StateClip(short clipId, short stateId)
	{
		auto prevNodeId = GetNodeId(NodeType::CLIP, clipId);
		auto nextNodeId = GetNodeId(NodeType::STATE, stateId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::CLIP, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::CLIP, 0);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::CLIP);
		nodeEditor.LayoutLink(prevNodeId, nextNodeId);
	}

	void NodeEditor::StateBlendNode(short blendNodeId, short stateId)
	{
		auto prevNodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
		auto nextNodeId = GetNodeId(NodeType::STATE, stateId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::BLEND_NODE, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::CLIP, 0);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::BLEND_NODE);
		nodeEditor.LayoutLink(prevNodeId, nextNodeId);
	}

	void NodeEditor::Variable(short variableId)
	{
		auto nodeId = GetNodeId(NodeType::VARIABLE, variableId);

		nodeEditor.BeginNode(nodeId, 10.0f);

		nodeEditor.BeginTitle();
		ImGui::Text("Variable");
		nodeEditor.EndTitle();

		nodeEditor.BeginInputPins();
		nodeEditor.EndInputPins();

		nodeEditor.BeginControls();
		Viewer("Name", asmData.variables[variableId]);
		if (gocAnimator)
			Viewer("Value", gocAnimator->animationStateMachine->variables[variableId].bindables.collectionFloat);
		nodeEditor.EndControls();

		nodeEditor.BeginOutputPins();
		OutputPin(nodeId, PinType::VARIABLE, 0);
		nodeEditor.EndOutputPins();

		nodeEditor.EndNode();
	}

	void NodeEditor::BlendNodeVariable(short variableId, short blendNodeId, unsigned short idx)
	{
		auto prevNodeId = GetNodeId(NodeType::VARIABLE, variableId);
		auto nextNodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::VARIABLE, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::VARIABLE, idx);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::VARIABLE);
		nodeEditor.LayoutLink(prevNodeId, nextNodeId);
	}

	void NodeEditor::BlendSpaceVariable(short variableId, short blendSpaceId, unsigned short idx)
	{
		auto prevNodeId = GetNodeId(NodeType::VARIABLE, variableId);
		auto nextNodeId = GetNodeId(NodeType::BLEND_SPACE, blendSpaceId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::VARIABLE, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::VARIABLE, idx);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::VARIABLE);
		nodeEditor.LayoutLink(prevNodeId, nextNodeId);
	}

	void NodeEditor::Clip(short clipId)
	{
		auto& clipData = asmData.clips[clipId];

		auto nodeId = GetNodeId(NodeType::CLIP, clipId);

		nodeEditor.BeginNode(nodeId, 10.0f);

		nodeEditor.BeginTitle();
		ImGui::Text("Clip");
		nodeEditor.EndTitle();

		nodeEditor.BeginInputPins();
		InputBlendMaskPin(nodeId, 0, "Blend mask", clipData.blendMaskIndex);
		nodeEditor.EndInputPins();

		nodeEditor.BeginControls();
		InputText("Name", clipData.name, &resource);
		InputText("Resource", clipData.animationSettings.resourceName, &resource);
		Editor("Start", clipData.animationSettings.start);
		Editor("End", clipData.animationSettings.end);
		Editor("Speed", clipData.animationSettings.speed);
		Editor("Loop", clipData.animationSettings.loops);
		CheckboxFlags("Mirror", clipData.animationSettings.flags, ClipData::AnimationSettings::Flag::MIRROR);
		CheckboxFlags("Play until end", clipData.animationSettings.flags, ClipData::AnimationSettings::Flag::PLAY_UNTIL_ANIMATION_END);
		CheckboxFlags("No resource resolution", clipData.animationSettings.flags, ClipData::AnimationSettings::Flag::NO_ANIMATION_RESOLUTION);
		nodeEditor.EndControls();

		nodeEditor.BeginOutputPins();
		OutputPin(nodeId, PinType::CLIP, 0);
		nodeEditor.EndOutputPins();

		nodeEditor.EndNode();
	}

	void NodeEditor::BlendNodeClip(short clipId, short blendNodeId, unsigned short idx)
	{
		auto prevNodeId = GetNodeId(NodeType::CLIP, clipId);
		auto nextNodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::CLIP, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::CLIP, idx);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::CLIP);
		nodeEditor.LayoutLink(prevNodeId, nextNodeId);
	}

	void NodeEditor::BlendSpaceClip(short clipId, short blendSpaceId, unsigned short idx)
	{
		auto prevNodeId = GetNodeId(NodeType::CLIP, clipId);
		auto nextNodeId = GetNodeId(NodeType::BLEND_SPACE, blendSpaceId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::CLIP, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::CLIP, idx);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::CLIP);
		nodeEditor.LayoutLink(prevNodeId, nextNodeId);
	}

	void NodeEditor::BlendSpace(short blendSpaceId)
	{
		auto& blendSpace = asmData.blendSpaces[blendSpaceId];

		auto nodeId = GetNodeId(NodeType::BLEND_SPACE, blendSpaceId);

		nodeEditor.BeginNode(nodeId, 10.0f);

		nodeEditor.BeginTitle();
		ImGui::Text("BlendSpace");
		nodeEditor.EndTitle();

		nodeEditor.BeginInputPins();
		for (unsigned short i = 0; i < blendSpace.nodeCount; i++)
			InputClipPin(nodeId, i, "Clip", blendSpace.clipIndices[i]);
		BlendSpaceVariablePins(nodeId, blendSpaceId, 0);
		nodeEditor.EndInputPins();

		nodeEditor.BeginControls();
		BlendSpaceControls(blendSpaceId);
		nodeEditor.EndControls();

		nodeEditor.BeginOutputPins();
		OutputPin(nodeId, PinType::BLEND_SPACE, 0);
		nodeEditor.EndOutputPins();

		nodeEditor.EndNode();
	}

	void NodeEditor::BlendNodeBlendSpace(short blendSpaceId, short blendNodeId)
	{
		auto prevNodeId = GetNodeId(NodeType::BLEND_SPACE, blendSpaceId);
		auto nextNodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::BLEND_SPACE, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::BLEND_SPACE, 0);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::BLEND_SPACE);
		nodeEditor.LayoutLink(prevNodeId, nextNodeId);
	}

	void NodeEditor::BlendMask(short blendMaskId)
	{
		auto& blendMask = asmData.blendMasks[blendMaskId];

		auto nodeId = GetNodeId(NodeType::BLEND_MASK, blendMaskId);

		nodeEditor.BeginNode(nodeId, 10.0f);

		nodeEditor.BeginTitle();
		ImGui::Text("Blend mask");
		nodeEditor.EndTitle();

		nodeEditor.BeginInputPins();
		nodeEditor.EndInputPins();

		nodeEditor.BeginControls();
		InputText("Name", blendMask.name, &resource);
		ImGui::Text("Bones:");
		for (unsigned short i = 0; i < blendMask.maskBoneCount; i++)
			ImGui::Text("%s", asmData.maskBones[blendMask.maskBoneOffset + i]);
		nodeEditor.EndControls();

		nodeEditor.BeginOutputPins();
		OutputPin(nodeId, PinType::BLEND_MASK, 0);
		nodeEditor.EndOutputPins();

		nodeEditor.EndNode();
	}

	void NodeEditor::ClipBlendMask(short blendMaskId, short clipId)
	{
		auto prevNodeId = GetNodeId(NodeType::BLEND_MASK, blendMaskId);
		auto nextNodeId = GetNodeId(NodeType::CLIP, clipId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::BLEND_MASK, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::BLEND_MASK, 0);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::BLEND_MASK);
		nodeEditor.LayoutLink(prevNodeId, nextNodeId);
	}

	void NodeEditor::LerpBlendNode(short blendNodeId, hh::anim::LerpBlendNode* liveNode)
	{
		BranchBlendNode(blendNodeId, "Lerp", [=]() {
			auto& nodeData = asmData.blendNodes[blendNodeId];

			BaseBlendNodeControls(blendNodeId);

			if (nodeData.childNodeArraySize < 2)
				return;

			auto childNodes = &asmData.blendNodes[nodeData.childNodeArrayOffset];

			if (ImPlot::BeginPlot("Lerp", { 300.0f, 200.0f })) {
				ImPlot::SetupAxes("Blend factor", "Contribution", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);

				for (unsigned short i = 0; i < nodeData.childNodeArraySize; i++) {
					std::pair<unsigned short, BlendNodeData*> plotInfo{ i, childNodes };
					char name[100];
					snprintf(name, sizeof(name), "%d", i);

					ImPlot::PlotLineG(name, [](auto idx, void* userData) -> ImPlotPoint {
						auto* pInfo = static_cast<std::pair<unsigned short, BlendNodeData*>*>(userData);

						return { pInfo->second[idx].blendFactorTarget, pInfo->first == idx ? 1.0f : 0.0f };
						}, &plotInfo, nodeData.childNodeArraySize);
				}

				if (gocAnimator && nodeData.blendFactorVariableIndex != -1) {
					double cur = gocAnimator->animationStateMachine->variables[nodeData.blendFactorVariableIndex].bindables.collectionFloat;
					double values[]{ cur };
					ImPlot::PlotInfLines("Current", values, 1);
				}

				ImPlot::EndPlot();
			}
		});
	}

	void NodeEditor::AdditiveBlendNode(short blendNodeId, hh::anim::AdditiveBlendNode* liveNode)
	{
		SimpleBranchBlendNode(blendNodeId, "Add");
	}

	void NodeEditor::ClipNode(short blendNodeId, hh::anim::ClipNode* liveNode)
	{
		BlendNode(
			blendNodeId,
			"Clip",
			[=]() {
				auto nodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
				auto& nodeData = asmData.blendNodes[blendNodeId];

				BaseBlendNodeInputs(blendNodeId);

				InputClipPin(nodeId, 0, "Clip", nodeData.childNodeArrayOffset);
			},
			[=]() { BaseBlendNodeControls(blendNodeId); }
		);
	}

	void NodeEditor::OverrideBlendNode(short blendNodeId, hh::anim::OverrideBlendNode* liveNode)
	{
		SimpleBranchBlendNode(blendNodeId, "Override");
	}

	void NodeEditor::LayerBlendNode(short blendNodeId, hh::anim::LayerBlendNode* liveNode)
	{
		SimpleBlendNode(blendNodeId, "Layer");
	}

	void NodeEditor::MulBlendNode(short blendNodeId, hh::anim::MulBlendNode* liveNode)
	{
		SimpleBranchBlendNode(blendNodeId, "Mul");
	}

	void NodeEditor::BlendSpaceNode(short blendNodeId, hh::anim::BlendSpaceNode* liveNode)
	{
		BlendNode(
			blendNodeId,
			"BlendSpace",
			[=]() {
				auto nodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
				auto& nodeData = asmData.blendNodes[blendNodeId];

				BaseBlendNodeInputs(blendNodeId);

				InputPin(nodeId, PinType::BLEND_SPACE, 0, "Blend space");

				if (nodeData.blendSpaceIndex != -1) {
					auto& blendSpace = asmData.blendSpaces[nodeData.blendSpaceIndex];

					for (unsigned short i = 0; i < blendSpace.nodeCount; i++)
						InputPin(nodeId, PinType::BLEND_NODE, i, asmData.clips[blendSpace.clipIndices[i]].name);
				}
			},
			[=]() { BaseBlendNodeControls(blendNodeId); }
		);
	}

	void NodeEditor::CollapsedBlendSpaceNode(short blendNodeId, hh::anim::BlendSpaceNode* liveNode)
	{
		BlendNode(
			blendNodeId,
			"BlendSpace",
			[=]() {
				auto nodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
				auto& nodeData = asmData.blendNodes[blendNodeId];

				BaseBlendNodeInputs(blendNodeId);

				BlendSpaceVariablePins(nodeId, nodeData.blendSpaceIndex, 1);

				if (nodeData.blendSpaceIndex != -1) {
					auto& blendSpace = asmData.blendSpaces[nodeData.blendSpaceIndex];

					for (unsigned short i = 0; i < blendSpace.nodeCount; i++)
						InputPin(nodeId, PinType::CLIP, i, asmData.clips[blendSpace.clipIndices[i]].name);
				}
			},
			[=]() {
				BaseBlendNodeControls(blendNodeId);
				BlendSpaceControls(asmData.blendNodes[blendNodeId].blendSpaceIndex);
			}
		);
	}

	void NodeEditor::TwoPointLerpBlendNode(short blendNodeId, hh::anim::TwoPointLerpBlendNode* liveNode)
	{
		SimpleBlendNode(blendNodeId, "Two Point Lerp");
	}

	void NodeEditor::SimpleBlendNode(short blendNodeId, const char* name)
	{
		BlendNode(
			blendNodeId,
			name,
			[=]() { BaseBlendNodeInputs(blendNodeId); },
			[=]() { BaseBlendNodeControls(blendNodeId); }
		);
	}

	void NodeEditor::SimpleBranchBlendNode(short blendNodeId, const char* name)
	{
		BranchBlendNode(
			blendNodeId,
			name,
			[=]() { BaseBlendNodeControls(blendNodeId); }
		);
	}

	void NodeEditor::BaseBlendNodeInputs(short blendNodeId)
	{
		auto nodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
		auto& nodeData = asmData.blendNodes[blendNodeId];

		InputVariablePin(nodeId, 0, "Blend factor", nodeData.blendFactorVariableIndex);
	}

	void NodeEditor::BaseBlendNodeControls(short blendNodeId)
	{
		auto& nodeData = asmData.blendNodes[blendNodeId];

		Editor("Blend factor target", nodeData.blendFactorTarget);
	}

	void NodeEditor::BlendNodeChildRelationship(short childNodeId, short parentNodeId, short idx)
	{
		auto prevNodeId = GetNodeId(NodeType::BLEND_NODE, childNodeId);
		auto nextNodeId = GetNodeId(NodeType::BLEND_NODE, parentNodeId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::BLEND_NODE, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::BLEND_NODE, idx);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::BLEND_NODE);
		nodeEditor.LayoutLink(prevNodeId, nextNodeId);
	}

	void NodeEditor::LayerBlendTreeOutput(short blendNodeId)
	{
		auto prevNodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
		auto nodeId = GetNodeId(NodeType::LAYER_BLEND_TREE_OUTPUT, 0);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::BLEND_NODE, 0);
		auto nextPin = GetInputPinId(nodeId, PinType::BLEND_NODE, 0);

		nodeEditor.BeginNode(nodeId, 10.0f);

		nodeEditor.BeginTitle();
		ImGui::Text("Layer blend tree output");
		nodeEditor.EndTitle();

		nodeEditor.BeginInputPins();
		InputPin(nodeId, PinType::BLEND_NODE, 0, "Blend node");
		nodeEditor.EndInputPins();

		nodeEditor.BeginControls();
		nodeEditor.EndControls();

		nodeEditor.BeginOutputPins();
		nodeEditor.EndOutputPins();

		nodeEditor.EndNode();

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::BLEND_NODE);
		nodeEditor.LayoutLink(prevNodeId, nodeId);
	}

	bool NodeEditor::IsStateSelected(unsigned short idx)
	{
		return ax::NodeEditor::IsNodeSelected(GetNodeId(NodeType::STATE, idx));
	}

	void NodeEditor::BeginInputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx)
	{
		nodeEditor.BeginInputPin(GetInputPinId(nodeId, type, idx), type);
	}

	void NodeEditor::EndInputPin() {
		nodeEditor.EndInputPin();
	}

	void NodeEditor::BeginOutputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx, float labelWidth)
	{
		nodeEditor.BeginOutputPin(GetOutputPinId(nodeId, type, idx), labelWidth, type);
	}

	void NodeEditor::EndOutputPin() {
		nodeEditor.EndOutputPin();
	}

	void NodeEditor::InputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx)
	{
		BeginInputPin(nodeId, type, idx);
		EndInputPin();
	}

	void NodeEditor::OutputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx)
	{
		BeginOutputPin(nodeId, type, idx, 0.0f);
		EndOutputPin();
	}

	void NodeEditor::InputVariablePin(ax::NodeEditor::NodeId nodeId, unsigned short idx, const char* label, short variableId)
	{
		if (gocAnimator && variableId != -1)
			InputPin(nodeId, PinType::VARIABLE, idx, label, "%f", gocAnimator->animationStateMachine->variables[variableId].bindables.collectionFloat);
		else
			InputPin(nodeId, PinType::VARIABLE, idx, label);
	}

	void NodeEditor::InputClipPin(ax::NodeEditor::NodeId nodeId, unsigned short idx, const char* label, short clipId)
	{
		if (clipId != -1)
			InputPin(nodeId, PinType::CLIP, idx, label, "%s", asmData.clips[clipId].name);
		else
			InputPin(nodeId, PinType::CLIP, idx, label);
	}

	void NodeEditor::InputBlendMaskPin(ax::NodeEditor::NodeId nodeId, unsigned short idx, const char* label, short blendMaskId)
	{
		if (blendMaskId != -1)
			InputPin(nodeId, PinType::BLEND_MASK, idx, label, "%s", asmData.blendMasks[blendMaskId].name);
		else
			InputPin(nodeId, PinType::BLEND_MASK, idx, label);
	}

	void NodeEditor::BlendSpaceVariablePins(ax::NodeEditor::NodeId nodeId, short blendSpaceId, unsigned short startIdx)
	{
		auto& blendSpace = asmData.blendSpaces[blendSpaceId];

		InputVariablePin(nodeId, startIdx + 0, "X", blendSpace.xVariableIndex);
		InputVariablePin(nodeId, startIdx + 1, "Y", blendSpace.yVariableIndex);
	}

	void NodeEditor::BlendSpaceControls(short blendSpaceId)
	{
		auto& blendSpace = asmData.blendSpaces[blendSpaceId];

		if (ImPlot::BeginPlot("Blend space", { 300.0f, 300.0f })) {
			ImPlot::SetupAxes(asmData.variables[blendSpace.xVariableIndex], asmData.variables[blendSpace.yVariableIndex], ImPlotAxisFlags_None, ImPlotAxisFlags_None);
			ImPlot::SetupAxesLimits(blendSpace.xMin, blendSpace.xMax, blendSpace.yMin, blendSpace.yMax);

			for (unsigned short i = 0; i < blendSpace.nodeCount; i++) {
				auto& node = blendSpace.nodes[i];
				double x = node.x();
				double y = node.y();

				if (ImPlot::DragPoint(i, &x, &y, { 1.0f, 1.0f, 1.0f, 1.0f }))
					node = { x, y };
				ImPlot::Annotation(x, y, { 1.0f, 1.0f, 1.0f, 0.0f }, { 0.0f, -5.0f }, true, "%s", asmData.clips[blendSpace.clipIndices[i]].name);
			}

			for (unsigned short i = 0; i < blendSpace.triangleCount; i++) {
				char name[100];
				snprintf(name, sizeof(name), "##%d", i);
				auto& triangle = blendSpace.triangles[i];
				double triangleXs[] = { blendSpace.nodes[triangle.nodeIndices[0]].x(), blendSpace.nodes[triangle.nodeIndices[1]].x(), blendSpace.nodes[triangle.nodeIndices[2]].x() };
				double triangleYs[] = { blendSpace.nodes[triangle.nodeIndices[0]].y(), blendSpace.nodes[triangle.nodeIndices[1]].y(), blendSpace.nodes[triangle.nodeIndices[2]].y() };
				ImPlot::PlotLine(name, triangleXs, triangleYs, 3, ImPlotLineFlags_Loop);
			}

			if (gocAnimator && blendSpace.xVariableIndex != -1 && blendSpace.yVariableIndex != -1) {
				double curX = gocAnimator->animationStateMachine->variables[blendSpace.xVariableIndex].bindables.collectionFloat;
				double curY = gocAnimator->animationStateMachine->variables[blendSpace.yVariableIndex].bindables.collectionFloat;

				ImPlot::PlotScatter("Current", &curX, &curY, 1, ImPlotScatterFlags_None);
			}

			ImPlot::EndPlot();
		}
	}

	ax::NodeEditor::NodeId NodeEditor::GetNodeId(NodeType type, unsigned short idx)
	{
		unsigned long long lType = static_cast<unsigned long long>(type);
		unsigned long long lIdx = idx;

		return (lType << 16) | lIdx;
	}

	ax::NodeEditor::PinId NodeEditor::GetPinId(ax::NodeEditor::NodeId nodeId, ax::NodeEditor::PinKind kind, PinType type, unsigned short idx)
	{
		unsigned long long lNodeId = nodeId.Get();
		unsigned long long lKind = static_cast<unsigned long long>(kind);
		unsigned long long lType = static_cast<unsigned long long>(type);
		unsigned long long lIdx = idx;

		return (lNodeId << 12) | (lKind << 11) | (lType << 7) | lIdx;
	}

	ax::NodeEditor::PinId NodeEditor::GetInputPinId(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx)
	{
		return GetPinId(nodeId, ax::NodeEditor::PinKind::Input, type, idx);
	}

	ax::NodeEditor::PinId NodeEditor::GetOutputPinId(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx)
	{
		return GetPinId(nodeId, ax::NodeEditor::PinKind::Output, type, idx);
	}

	ax::NodeEditor::LinkId NodeEditor::GetLinkId(ax::NodeEditor::PinId fromPinId, ax::NodeEditor::PinId toPinId) {
		unsigned long long lFromPinId = fromPinId.Get();
		unsigned long long lToPinId = toPinId.Get();

		return (lFromPinId << 32) | lToPinId;
	}
}
