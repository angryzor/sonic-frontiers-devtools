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
	}

	void BlendTreeEditor::RenderContents()
	{
		auto& asmData = *gocAnimator->asmResourceManager->animatorResource->binaryData;

		nodeEditor.Begin();
		for (unsigned short i = 0; i < asmData.variableCount; i++)
			RenderVariable(i);
		for (unsigned short i = 0; i < asmData.stateCount; i++) {
			auto& state = asmData.states[i];

			if (state.type == StateType::BLEND_TREE)
				RenderNode(nullptr, state.rootBlendNodeOrClipIndex);
		}
		RenderNode(focusedRootBlendNode, focusedRootBlendNodeIndex);
		nodeEditor.End();
	}

	void BlendTreeEditor::RenderVariable(short variableId)
	{
		nodeEditor.Variable(variableId);
	}

	void BlendTreeEditor::RenderNode(hh::anim::BlendNodeBase* node, short nodeId)
	{
		auto& blendNodeData = gocAnimator->asmResourceManager->animatorResource->binaryData->blendNodes[nodeId];

		for (unsigned short i = 0; i < blendNodeData.childNodeArraySize; i++)
			RenderNode(node == nullptr ? nullptr : node->children[i], blendNodeData.childNodeArrayOffset + i);

		switch (blendNodeData.type) {
		case BlendNodeType::LERP: nodeEditor.BlendNode(nodeId, static_cast<LerpBlendNode*>(node)); break;
		case BlendNodeType::ADDITIVE: nodeEditor.BlendNode(nodeId, static_cast<AdditiveBlendNode*>(node)); break;
		case BlendNodeType::CLIP: nodeEditor.BlendNode(nodeId, static_cast<ClipNode*>(node)); break;
		case BlendNodeType::OVERRIDE: nodeEditor.BlendNode(nodeId, static_cast<OverrideBlendNode*>(node)); break;
		case BlendNodeType::LAYER: nodeEditor.BlendNode(nodeId, static_cast<LayerBlendNode*>(node)); break;
		case BlendNodeType::MULTIPLY: nodeEditor.BlendNode(nodeId, static_cast<MulBlendNode*>(node)); break;
		case BlendNodeType::BLEND_SPACE: nodeEditor.BlendNode(nodeId, static_cast<BlendSpaceNode*>(node)); break;
		case BlendNodeType::TWO_POINT_LERP: nodeEditor.BlendNode(nodeId, static_cast<TwoPointLerpBlendNode*>(node)); break;
		default: assert(false);
		}

		for (unsigned short i = 0; i < blendNodeData.childNodeArraySize; i++)
			nodeEditor.BlendNodeChildRelationship(blendNodeData.childNodeArrayOffset + i, nodeId, i);

		if (blendNodeData.variableIndex != -1)
			nodeEditor.BlendNodeVariable(blendNodeData.variableIndex, nodeId, 0);

		if (blendNodeData.type == BlendNodeType::BLEND_SPACE && blendNodeData.blendSpaceIndex != -1) {
			auto& blendSpace = gocAnimator->asmResourceManager->animatorResource->binaryData->blendSpaces[blendNodeData.blendSpaceIndex];

			if (blendSpace.xVariableIndex != -1)
				nodeEditor.BlendNodeVariable(blendSpace.xVariableIndex, nodeId, 1);
			if (blendSpace.yVariableIndex != -1)
				nodeEditor.BlendNodeVariable(blendSpace.yVariableIndex, nodeId, 2);
		}
	}

	//void BlendTreeEditor::RenderNodeLinks(hh::anim::LayerBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	//{
	//	auto& layer = gocAnimator->animationStateMachine->layers[nodeData.childNodeArrayOffset];

	//	if (AnimationStateMachine::LayerStateBase* layerState = layer.layerState) {
	//		auto* nextAnimationState = layerState->GetNextAnimationState();
	//		auto* prevAnimationState = layerState->GetPreviousAnimationState();

	//		if (nextAnimationState && nextAnimationState->stateData->type == StateType::BLEND_TREE) {
	//			nodeEditor.Link(GetLinkId(nextAnimationState->stateData->rootBlendNodeOrClipIndex, 0, nodeId, 0), GetOutputPinId(nextAnimationState->stateData->rootBlendNodeOrClipIndex, 0), GetInputPinId(nodeId, 0));
	//			if (prevAnimationState && nextAnimationState != prevAnimationState)
	//				nodeEditor.Flow(GetLinkId(nextAnimationState->stateData->rootBlendNodeOrClipIndex, 0, nodeId, 0), NodeEd::FlowDirection::Backward);
	//		}

	//		if (prevAnimationState && prevAnimationState->stateData->type == StateType::BLEND_TREE) {
	//			nodeEditor.Link(GetLinkId(prevAnimationState->stateData->rootBlendNodeOrClipIndex, 0, nodeId, 0), GetOutputPinId(prevAnimationState->stateData->rootBlendNodeOrClipIndex, 0), GetInputPinId(nodeId, 0));
	//			if (nextAnimationState && nextAnimationState != prevAnimationState)
	//				nodeEditor.Flow(GetLinkId(prevAnimationState->stateData->rootBlendNodeOrClipIndex, 0, nodeId, 0));
	//		}
	//	}
	//}
}
