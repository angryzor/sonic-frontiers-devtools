#include "BlendTreeEditor.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

namespace ui::operation_modes::modes::asm_editor {
	using namespace hh::anim;
	namespace NodeEd = ax::NodeEditor;

	const char* nodeNames[]{ "Lerp", "Add", "Clip", "Override", "Layer", "Multiply", "BlendSpace", "Two Point Lerp" };

	BlendTreeEditor::BlendTreeEditor(csl::fnd::IAllocator* allocator, GOCAnimator* gocAnimator, BlendNodeBase* focusedRootBlendNode, short focusedRootBlendNodeIndex) : StandaloneWindow{ allocator }, gocAnimator{ gocAnimator }, focusedRootBlendNode { focusedRootBlendNode }, focusedRootBlendNodeIndex{ focusedRootBlendNodeIndex }
	{
		char title[400];
		snprintf(title, sizeof(title), "Blend tree @ %016zx", focusedRootBlendNode);
		SetTitle(title);

		ax::NodeEditor::Config config{};
		context = NodeEd::CreateEditor(&config);
	}

	BlendTreeEditor::~BlendTreeEditor() {
		NodeEd::DestroyEditor(context);
	}

	void BlendTreeEditor::RenderContents()
	{
		auto& asmData = *gocAnimator->asmResourceManager->animatorResource->binaryData;

		NodeEd::SetCurrentEditor(context);
		NodeEd::Begin("Editor");
		for (unsigned short i = 0; i < asmData.stateCount; i++) {
			auto& state = asmData.states[i];

			if (state.type == StateType::BLEND_TREE)
				RenderNode(nullptr, state.rootBlendNodeOrClipIndex);
		}
		RenderNode(focusedRootBlendNode, focusedRootBlendNodeIndex);
		NodeEd::End();
		NodeEd::SetCurrentEditor(nullptr);
	}

	void BlendTreeEditor::RenderNodeControls(hh::anim::BlendSpaceNode* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	{
		RenderNodeControls(static_cast<hh::anim::BlendNodeBase*>(node), nodeId, nodeData);

		auto& asmData = *gocAnimator->asmResourceManager->animatorResource->binaryData;
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

			double curX = gocAnimator->animationStateMachine->variables[blendSpace.xVariableIndex].bindables.collectionFloat;
			double curY = gocAnimator->animationStateMachine->variables[blendSpace.yVariableIndex].bindables.collectionFloat;

			ImPlot::PlotScatter("Current", &curX, &curY, 1, ImPlotScatterFlags_None);

			ImPlot::EndPlot();
		}
	}

	void BlendTreeEditor::RenderNode(hh::anim::BlendNodeBase* node, short nodeId)
	{
		auto& blendNodeData = gocAnimator->asmResourceManager->animatorResource->binaryData->blendNodes[nodeId];

		switch (blendNodeData.type) {
		case BlendNodeType::LERP: RenderNode(static_cast<LerpBlendNode*>(node), nodeId, blendNodeData); break;
		case BlendNodeType::ADDITIVE: RenderNode(static_cast<AdditiveBlendNode*>(node), nodeId, blendNodeData); break;
		case BlendNodeType::CLIP: RenderNode(static_cast<ClipNode*>(node), nodeId, blendNodeData); break;
		case BlendNodeType::OVERRIDE: RenderNode(static_cast<OverrideBlendNode*>(node), nodeId, blendNodeData); break;
		case BlendNodeType::LAYER: RenderNode(static_cast<LayerBlendNode*>(node), nodeId, blendNodeData); break;
		case BlendNodeType::MULTIPLY: RenderNode(static_cast<MulBlendNode*>(node), nodeId, blendNodeData); break;
		case BlendNodeType::BLEND_SPACE: RenderNode(static_cast<BlendSpaceNode*>(node), nodeId, blendNodeData); break;
		case BlendNodeType::TWO_POINT_LERP: RenderNode(static_cast<TwoPointLerpBlendNode*>(node), nodeId, blendNodeData); break;
		default: assert(false);
		}
	}

	void BlendTreeEditor::RenderNodeChildren(hh::anim::BranchBlendNode* node, short nodeId, BlendNodeData& nodeData)
	{
		for (unsigned short i = 0; i < nodeData.childNodeArraySize; i++)
			RenderNode(node == nullptr ? nullptr : node->children[i], nodeData.childNodeArrayOffset + i);
	}

	void BlendTreeEditor::RenderNodeChildren(hh::anim::ClipNode* node, short nodeId, BlendNodeData& nodeData)
	{
	}

	void BlendTreeEditor::RenderNodeChildren(hh::anim::LayerBlendNode* node, short nodeId, BlendNodeData& nodeData)
	{
		//auto& layer = gocAnimator->animationStateMachine->layers[nodeData.childNodeArrayOffset];

		//if (AnimationStateMachine::LayerStateBase* layerState = layer.layerState)
		//if (auto* animationState = layerState->GetNextAnimationState())
		//	RenderNode(*animationState->implementation.blendTree, animationState->stateData->rootBlendNodeIndex);
	}

	void BlendTreeEditor::RenderNodeTitle(hh::anim::BlendNodeBase* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	{
		ImGui::Text(nodeNames[static_cast<uint8_t>(nodeData.type)]);
	}

	void BlendTreeEditor::RenderNodeControls(hh::anim::BlendNodeBase* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	{
		Editor("Blend factor", nodeData.blendFactor);
	}

	void BlendTreeEditor::RenderNodeControls(hh::anim::ClipNode* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	{
		RenderNodeControls(static_cast<hh::anim::BlendNodeBase*>(node), nodeId, nodeData);

		auto& clipData = gocAnimator->asmResourceManager->animatorResource->binaryData->clips[nodeData.childNodeArrayOffset];
		Viewer("Animation resource", clipData.name);
		Editor("Animation start", clipData.animationSettings.start);
		Editor("Animation end", clipData.animationSettings.end);
		Editor("Animation speed", clipData.animationSettings.speed);
		Editor("Looping", clipData.animationSettings.loops);
	}

	void BlendTreeEditor::RenderNodeInputPins(hh::anim::BlendNodeBase* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	{
	}

	void BlendTreeEditor::RenderNodeInputPins(hh::anim::LayerBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	{
		NodeEd::BeginPin(GetInputPinId(nodeId, 0), NodeEd::PinKind::Input);
		ImGui::Text("State blend tree");
		NodeEd::EndPin();
	}

	void BlendTreeEditor::RenderNodeInputPins(hh::anim::BranchBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	{
		for (unsigned short i = 0; i < nodeData.childNodeArraySize; i++) {
			NodeEd::BeginPin(GetInputPinId(nodeId, i), NodeEd::PinKind::Input);
			ImGui::Text("In");
			NodeEd::EndPin();
		}
	}

	void BlendTreeEditor::RenderNodeOutputPins(hh::anim::BlendNodeBase* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 200.0f - ImGui::CalcTextSize("Out").x);
		NodeEd::BeginPin(GetOutputPinId(nodeId, 0), NodeEd::PinKind::Output);
		NodeEd::PinPivotAlignment({ 1.0f, 0.5f });
		NodeEd::PinPivotSize({ 0.0f, 0.0f });
		ImGui::Text("Out");
		NodeEd::EndPin();
	}

	void BlendTreeEditor::RenderNodeLinks(hh::anim::BlendNodeBase* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	{
	}

	void BlendTreeEditor::RenderNodeLinks(hh::anim::BranchBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	{
		for (unsigned short i = 0; i < nodeData.childNodeArraySize; i++)
			NodeEd::Link(GetLinkId(nodeData.childNodeArrayOffset + i, 0, nodeId, i), GetOutputPinId(nodeData.childNodeArrayOffset + i, 0), GetInputPinId(nodeId, i));
	}

	void BlendTreeEditor::RenderNodeLinks(hh::anim::LayerBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	{
		auto& layer = gocAnimator->animationStateMachine->layers[nodeData.childNodeArrayOffset];

		if (AnimationStateMachine::LayerStateBase* layerState = layer.layerState)
			if (auto* animationState = layerState->GetNextAnimationState())
				NodeEd::Link(GetLinkId(animationState->stateData->rootBlendNodeOrClipIndex, 0, nodeId, 0), GetOutputPinId(animationState->stateData->rootBlendNodeOrClipIndex, 0), GetInputPinId(nodeId, 0));
	}

	ax::NodeEditor::NodeId BlendTreeEditor::GetNodeId(short nodeId)
	{
		return nodeId;
	}

	ax::NodeEditor::PinId BlendTreeEditor::GetInputPinId(short nodeId, unsigned short idx)
	{
		return (nodeId << 17) | (0 << 16) | idx;
	}

	ax::NodeEditor::PinId BlendTreeEditor::GetOutputPinId(short nodeId, unsigned short idx)
	{
		return (nodeId << 17) | (1 << 16) | idx;
	}
	ax::NodeEditor::LinkId BlendTreeEditor::GetLinkId(short fromNodeId, unsigned short fromOutputPinIdx, short toNodeId, unsigned short toOutputPinIdx)
	{
		return (((fromNodeId << 16) | fromOutputPinIdx) << 32) | ((toNodeId << 16) | toOutputPinIdx);
	}
}
