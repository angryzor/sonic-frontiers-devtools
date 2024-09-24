#include "BlendTreeEditor.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/GlobalSettings.h>
#include <io/binary/containers/binary-file/BinaryFile.h>
#include <io/binary/serialization/resource-rfls/ResAnimator.h>

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

		if (ImGui::BeginMenuBar()) {
			if (ImGui::MenuItem("Export")) {
				IGFD::FileDialogConfig cfg{};
				cfg.path = GlobalSettings::defaultFileDialogDirectory;
				cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
				cfg.userDatas = &asmData;
				ImGuiFileDialog::Instance()->OpenDialog("ResAnimatorExportDialog", "Choose File", ".asm", cfg);
			}
			if (ImGui::MenuItem("Toggle collapse blend space nodes"))
				collapseBlendSpaceNodes = !collapseBlendSpaceNodes;
			if (ImGui::MenuItem("Run auto-layout"))
				nodeEditor.RunAutoLayout();
			ImGui::EndMenuBar();
		}

		if (ImGuiFileDialog::Instance()->Display("ResAnimatorExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				auto* exportData = static_cast<hh::anim::AsmData*>(ImGuiFileDialog::Instance()->GetUserDatas());

				std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
				std::wstring wFilePath(filePath.begin(), filePath.end());

				devtools::io::binary::containers::BinaryFile::Serialize(wFilePath.c_str(), exportData, &hh::fnd::RflClassTraits<hh::anim::AsmData>::rflClass);
			}
			ImGuiFileDialog::Instance()->Close();
		}

		nodeEditor.Begin();
		for (unsigned short i = 0; i < asmData.variableCount; i++)
			RenderVariable(i);
		for (unsigned short i = 0; i < asmData.blendMaskCount; i++)
			RenderBlendMask(i);
		for (unsigned short i = 0; i < asmData.clipCount; i++)
			RenderClip(i);
		if (!collapseBlendSpaceNodes)
			for (unsigned short i = 0; i < asmData.blendSpaceCount; i++)
				RenderBlendSpace(i);
		for (unsigned short i = 0; i < asmData.stateCount; i++)
			RenderState(i);
		RenderNode(focusedRootBlendNode, focusedRootBlendNodeIndex);
		nodeEditor.LayerBlendTreeOutput(focusedRootBlendNodeIndex);
		nodeEditor.End();
	}

	void BlendTreeEditor::RenderVariable(short variableId)
	{
		nodeEditor.Variable(variableId);
	}

	void BlendTreeEditor::RenderBlendMask(short blendMaskId)
	{
		nodeEditor.BlendMask(blendMaskId);
	}

	void BlendTreeEditor::RenderClip(short clipId)
	{
		auto& clipData = gocAnimator->asmResourceManager->animatorResource->binaryData->clips[clipId];

		nodeEditor.Clip(clipId);

		if (clipData.blendMaskIndex != -1)
			nodeEditor.ClipBlendMask(clipData.blendMaskIndex, clipId);
	}

	void BlendTreeEditor::RenderState(short stateId)
	{
		auto& state = gocAnimator->asmResourceManager->animatorResource->binaryData->states[stateId];

		if (state.type == StateType::BLEND_TREE)
			RenderNode(nullptr, state.rootBlendNodeOrClipIndex);

		nodeEditor.State(stateId, ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeBorder], 0.0f);

		switch (state.type) {
		case StateType::BLEND_TREE:
			RenderNode(nullptr, state.rootBlendNodeOrClipIndex);
			nodeEditor.StateBlendNode(state.rootBlendNodeOrClipIndex, stateId);
			break;
		case StateType::CLIP:
			nodeEditor.StateClip(state.rootBlendNodeOrClipIndex, stateId);
			break;
		}
	}

	void BlendTreeEditor::RenderBlendSpace(short blendSpaceId)
	{
		auto& blendSpaceData = gocAnimator->asmResourceManager->animatorResource->binaryData->blendSpaces[blendSpaceId];

		nodeEditor.BlendSpace(blendSpaceId);

		if (blendSpaceData.xVariableIndex != -1)
			nodeEditor.BlendSpaceVariable(blendSpaceData.xVariableIndex, blendSpaceId, 0);

		if (blendSpaceData.yVariableIndex != -1)
			nodeEditor.BlendSpaceVariable(blendSpaceData.yVariableIndex, blendSpaceId, 1);

		for (unsigned short i = 0; i < blendSpaceData.nodeCount; i++)
			nodeEditor.BlendSpaceClip(blendSpaceData.clipIndices[i], blendSpaceId, i);
	}

	void BlendTreeEditor::RenderNode(hh::anim::BlendNodeBase* node, short nodeId)
	{
		auto& asmData = *gocAnimator->asmResourceManager->animatorResource->binaryData;
		auto& blendNodeData = asmData.blendNodes[nodeId];

		if (blendNodeData.type != BlendNodeType::BLEND_SPACE || !collapseBlendSpaceNodes)
			for (unsigned short i = 0; i < blendNodeData.childNodeArraySize; i++)
				RenderNode(node == nullptr ? nullptr : node->children[i], blendNodeData.childNodeArrayOffset + i);

		switch (blendNodeData.type) {
		case BlendNodeType::LERP: nodeEditor.LerpBlendNode(nodeId, static_cast<LerpBlendNode*>(node)); break;
		case BlendNodeType::ADDITIVE: nodeEditor.AdditiveBlendNode(nodeId, static_cast<AdditiveBlendNode*>(node)); break;
		case BlendNodeType::CLIP: nodeEditor.ClipNode(nodeId, static_cast<ClipNode*>(node)); break;
		case BlendNodeType::OVERRIDE: nodeEditor.OverrideBlendNode(nodeId, static_cast<OverrideBlendNode*>(node)); break;
		case BlendNodeType::LAYER: nodeEditor.LayerBlendNode(nodeId, static_cast<LayerBlendNode*>(node)); break;
		case BlendNodeType::MULTIPLY: nodeEditor.MulBlendNode(nodeId, static_cast<MulBlendNode*>(node)); break;
		case BlendNodeType::TWO_POINT_LERP: nodeEditor.TwoPointLerpBlendNode(nodeId, static_cast<TwoPointLerpBlendNode*>(node)); break;
		case BlendNodeType::BLEND_SPACE:
			if (!collapseBlendSpaceNodes)
				nodeEditor.BlendSpaceNode(nodeId, static_cast<BlendSpaceNode*>(node));
			else
				nodeEditor.CollapsedBlendSpaceNode(nodeId, static_cast<BlendSpaceNode*>(node));
			break;
		default: assert(false);
		}

		if (blendNodeData.type != BlendNodeType::BLEND_SPACE || !collapseBlendSpaceNodes)
			for (unsigned short i = 0; i < blendNodeData.childNodeArraySize; i++)
				nodeEditor.BlendNodeChildRelationship(blendNodeData.childNodeArrayOffset + i, nodeId, i);

		if (blendNodeData.blendFactorVariableIndex != -1)
			nodeEditor.BlendNodeVariable(blendNodeData.blendFactorVariableIndex, nodeId, 0);

		if (blendNodeData.type == BlendNodeType::CLIP && blendNodeData.childNodeArrayOffset != -1)
			nodeEditor.BlendNodeClip(blendNodeData.childNodeArrayOffset, nodeId, 0);

		if (blendNodeData.type == BlendNodeType::BLEND_SPACE && blendNodeData.blendSpaceIndex != -1) {
			if (!collapseBlendSpaceNodes)
				nodeEditor.BlendNodeBlendSpace(blendNodeData.blendSpaceIndex, nodeId);
			else {
				auto& blendSpaceData = asmData.blendSpaces[blendNodeData.blendSpaceIndex];

				if (blendSpaceData.xVariableIndex != -1)
					nodeEditor.BlendNodeVariable(blendSpaceData.xVariableIndex, nodeId, 1);

				if (blendSpaceData.yVariableIndex != -1)
					nodeEditor.BlendNodeVariable(blendSpaceData.yVariableIndex, nodeId, 2);

				for (unsigned short i = 0; i < blendSpaceData.nodeCount; i++)
					nodeEditor.BlendNodeClip(blendSpaceData.clipIndices[i], nodeId, i);
			}
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
