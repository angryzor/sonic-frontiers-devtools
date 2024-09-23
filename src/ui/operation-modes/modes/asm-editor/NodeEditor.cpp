#include "NodeEditor.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

namespace ui::operation_modes::modes::asm_editor {
	using namespace hh::anim;
	namespace NodeEd = ax::NodeEditor;

	const char* NodeEditor::blendNodeNames[8]{ "Lerp", "Add", "Clip", "Override", "Layer", "Multiply", "BlendSpace", "Two Point Lerp" };

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
		default: {
			assert(false);
			break;
		}
		}
	}

	NodeEditor::NodeEditor(csl::fnd::IAllocator* allocator, hh::anim::ResAnimator& resource, hh::anim::GOCAnimator* gocAnimator) : CompatibleObject{ allocator }, nodeEditor{ allocator }, asmData { *resource.binaryData }, gocAnimator{ gocAnimator } {}

	void NodeEditor::Begin() {
		nodeEditor.Begin();
	}

	void NodeEditor::End() {
		nodeEditor.End();
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
		nodeEditor.EndInputPins();

		nodeEditor.BeginControls();
		ImGui::ProgressBar(progress, { ImGui::CalcTextSize(state.name).x + 10.0f, 0.0f }, state.name);
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

	void NodeEditor::BlendNodeInputs(short blendNodeId, hh::anim::BlendNodeBase* liveNode)
	{
		auto nodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
		auto& nodeData = asmData.blendNodes[blendNodeId];

		BeginInputPin(nodeId, PinType::VARIABLE, 0);
		if (nodeData.variableIndex == -1)
			Editor("Blend factor", nodeData.blendFactor);
		else
			ImGui::Text("Blend factor");
		EndInputPin();
	}

	void NodeEditor::BlendNodeInputs(short blendNodeId, hh::anim::BlendSpaceNode* liveNode)
	{
		auto nodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);

		BlendNodeInputs(blendNodeId, static_cast<hh::anim::BlendNodeBase*>(liveNode));

		InputPin(nodeId, PinType::VARIABLE, 1, "X");
		InputPin(nodeId, PinType::VARIABLE, 2, "Y");
	}

	void NodeEditor::BlendNodeControls(short blendNodeId, hh::anim::BlendNodeBase* liveNode)
	{
	}

	void NodeEditor::BlendNodeControls(short blendNodeId, hh::anim::ClipNode* liveNode)
	{
		BlendNodeControls(blendNodeId, static_cast<hh::anim::BlendNodeBase*>(liveNode));

		auto& nodeData = asmData.blendNodes[blendNodeId];
		auto& clipData = asmData.clips[nodeData.childNodeArrayOffset];

		Viewer("Animation resource", clipData.name);
		CheckboxFlags("Mirror", clipData.animationSettings.flags, ClipData::AnimationSettings::Flag::MIRROR);
		Editor("Animation start", clipData.animationSettings.start);
		Editor("Animation end", clipData.animationSettings.end);
		Editor("Animation speed", clipData.animationSettings.speed);
		Editor("Looping", clipData.animationSettings.loops);
	}

	void NodeEditor::BlendNodeControls(short blendNodeId, hh::anim::BlendSpaceNode* liveNode)
	{
		BlendNodeControls(blendNodeId, static_cast<hh::anim::BlendNodeBase*>(liveNode));

		auto& nodeData = asmData.blendNodes[blendNodeId];
		auto& blendSpace = asmData.blendSpaces[nodeData.blendSpaceIndex];

		if (ImPlot::BeginPlot("BlendSpace", { 300.0f, 300.0f })) {
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

	void NodeEditor::BlendNodeChildRelationship(short childNodeId, short parentNodeId, short idx)
	{
		auto prevNodeId = GetNodeId(NodeType::BLEND_NODE, childNodeId);
		auto nextNodeId = GetNodeId(NodeType::BLEND_NODE, parentNodeId);
		auto prevPin = GetOutputPinId(prevNodeId, PinType::BLEND_NODE, 0);
		auto nextPin = GetInputPinId(nextNodeId, PinType::BLEND_NODE, idx);

		nodeEditor.Link(GetLinkId(prevPin, nextPin), prevPin, nextPin, PinType::BLEND_NODE);
		nodeEditor.LayoutLink(prevNodeId, nextNodeId);
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

	void NodeEditor::InputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx, const char* text)
	{
		BeginInputPin(nodeId, type, idx);
		ImGui::Text("%s", text);
		EndInputPin();
	}

	void NodeEditor::InputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx)
	{
		BeginInputPin(nodeId, type, idx);
		EndInputPin();
	}

	void NodeEditor::OutputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx, const char* text)
	{
		BeginOutputPin(nodeId, type, idx, ImGui::CalcTextSize(text).x + ImGui::GetStyle().ItemSpacing.x);
		ImGui::Text("%s", text);
		EndOutputPin();
	}

	void NodeEditor::OutputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx)
	{
		BeginOutputPin(nodeId, type, idx, 0.0f);
		EndOutputPin();
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
