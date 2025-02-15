#include "BlendTreeEditor.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/GlobalSettings.h>
#ifdef DEVTOOLS_TARGET_SDK_rangers
#include <ucsl-reflection/reflections/resources/asm/v103-rangers.h>
#endif
#ifdef DEVTOOLS_TARGET_SDK_miller
#include <ucsl-reflection/reflections/resources/asm/v103-miller.h>
#endif
#include <rip/binary/containers/binary-file/v2.h>
#include <span>

namespace ui::operation_modes::modes::asm_editor {
	using namespace hh::anim;
	namespace NodeEd = ax::NodeEditor;

	const char* nodeNames[]{ "Lerp", "Add", "Clip", "Override", "Layer", "Multiply", "BlendSpace", "Two Point Lerp" };

	BlendTreeEditor::BlendTreeEditor(csl::fnd::IAllocator* allocator, GOCAnimator* gocAnimator, BlendNodeBase* focusedRootBlendNode, short focusedRootBlendNodeIndex) : StandaloneWindow{ allocator }, gocAnimator{ gocAnimator }, focusedRootBlendNode { focusedRootBlendNode }, focusedRootBlendNodeIndex{ focusedRootBlendNodeIndex }
	{
		char title[400];
		snprintf(title, sizeof(title), "Blend tree @ %016zx", focusedRootBlendNode);
		SetTitle(title);
		timelineCtx = ImTimeline::CreateContext();
	}

	BlendTreeEditor::~BlendTreeEditor()
	{
		ImTimeline::DestroyContext(timelineCtx);
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
#ifdef DEVTOOLS_TARGET_SDK_rangers
				auto* exportData = static_cast<ucsl::resources::animation_state_machine::v103_rangers::AsmData*>(ImGuiFileDialog::Instance()->GetUserDatas());
#endif
#ifdef DEVTOOLS_TARGET_SDK_miller
				auto* exportData = static_cast<ucsl::resources::animation_state_machine::v103_miller::AsmData*>(ImGuiFileDialog::Instance()->GetUserDatas());
#endif

				std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::binary };
				rip::binary::containers::binary_file::v2::BinaryFileSerializer<size_t> serializer{ ofs };
				serializer.serialize<he2sdk::ucsl::GameInterface>(*exportData);
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
		for (unsigned short i = 0; i < asmData.flagCount; i++)
			RenderFlag(i);
		if (!collapseBlendSpaceNodes)
			for (unsigned short i = 0; i < asmData.blendSpaceCount; i++)
				RenderBlendSpace(i);
		for (unsigned short i = 0; i < asmData.stateCount; i++)
			RenderState(i);

		// TODO: Game instead selects node 0 if this happens.
		if (focusedRootBlendNodeIndex != -1) {
			RenderNode(focusedRootBlendNode, focusedRootBlendNodeIndex);
			nodeEditor.LayerBlendTreeOutput(focusedRootBlendNodeIndex);
		}

		if (nodeEditor.BeginCreate()) {
			OutputPinId inPin;
			InputPinId outPin;

			if (nodeEditor.QueryNewLink(inPin, outPin)) {
				if (ax::NodeEditor::AcceptNewItem()) {
					if (IsType(inPin, outPin, NodeType::CLIP, NodeType::BLEND_NODE, PinType::CLIP)) {
						auto& blendNode = asmData.blendNodes[outPin.nodeId.idx];

						if (collapseBlendSpaceNodes && blendNode.type == BlendNodeType::BLEND_SPACE) {
							auto& childNode = asmData.blendNodes[blendNode.childNodeArrayOffset + outPin.idx];

							if (childNode.type == BlendNodeType::CLIP) {
								asmData.blendSpaces[blendNode.blendSpaceIndex].clipIndices[outPin.idx] = inPin.nodeId.idx;
								childNode.childNodeArrayOffset = inPin.nodeId.idx;
							}
							else
								ShowError("The clip in this slot in the blend space does not correspond to a clip node. Toggle collapsing of BlendSpaceNode nodes off to edit it.");
						}
						else if (blendNode.type == BlendNodeType::CLIP)
							blendNode.childNodeArrayOffset = inPin.nodeId.idx;
					}
					else if (IsType(inPin, outPin, NodeType::CLIP, NodeType::BLEND_SPACE, PinType::CLIP)) {
						asmData.blendSpaces[outPin.nodeId.idx].clipIndices[outPin.idx] = inPin.nodeId.idx;
					}
					else if (IsType(inPin, outPin, NodeType::CLIP, NodeType::STATE, PinType::CLIP)) {
						asmData.states[outPin.nodeId.idx].rootBlendNodeOrClipIndex = inPin.nodeId.idx;
						asmData.states[outPin.nodeId.idx].type = StateType::CLIP;
					}
					else if (IsType(inPin, outPin, NodeType::BLEND_NODE, PinType::BLEND_NODE, NodeType::STATE, PinType::CLIP)) {
						asmData.states[outPin.nodeId.idx].rootBlendNodeOrClipIndex = inPin.nodeId.idx;
						asmData.states[outPin.nodeId.idx].type = StateType::BLEND_TREE;
					}
					else if (IsType(inPin, outPin, NodeType::BLEND_SPACE, NodeType::BLEND_NODE, PinType::BLEND_SPACE)) {
						asmData.blendNodes[outPin.nodeId.idx].blendSpaceIndex = inPin.nodeId.idx;
					}
					else if (IsType(inPin, outPin, NodeType::BLEND_MASK, NodeType::CLIP, PinType::BLEND_MASK)) {
						asmData.clips[outPin.nodeId.idx].blendMaskIndex = inPin.nodeId.idx;
					}
					else if (IsType(inPin, outPin, NodeType::VARIABLE, NodeType::BLEND_NODE, PinType::VARIABLE)) {
						auto& blendNode = asmData.blendNodes[outPin.nodeId.idx];

						if (outPin.idx == 0)
							blendNode.blendFactorVariableIndex = inPin.nodeId.idx;
						else if (collapseBlendSpaceNodes && blendNode.type == BlendNodeType::BLEND_SPACE) {
							if (outPin.idx == 1)
								asmData.blendSpaces[blendNode.blendSpaceIndex].xVariableIndex = inPin.nodeId.idx;
							else if (outPin.idx == 2)
								asmData.blendSpaces[blendNode.blendSpaceIndex].yVariableIndex = inPin.nodeId.idx;
						}

					}
					else if (IsType(inPin, outPin, NodeType::VARIABLE, NodeType::BLEND_SPACE, PinType::VARIABLE)) {
						if (outPin.idx == 0)
							asmData.blendSpaces[outPin.nodeId.idx].xVariableIndex = inPin.nodeId.idx;
						else if (outPin.idx == 1)
							asmData.blendSpaces[outPin.nodeId.idx].yVariableIndex = inPin.nodeId.idx;
					}
				}
			}
		}

		ax::NodeEditor::Suspend();
		if (ImGui::BeginPopupModal("ErrorMsg")) {
			ImGui::Text("%s", errMsg);

			if (ImGui::Button("OK"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		ax::NodeEditor::Resume();
		
		ax::NodeEditor::NodeId ids[1];
		auto selectedCount = ax::NodeEditor::GetSelectedNodes(ids, 1);

		nodeEditor.EndCreate();

		nodeEditor.End();

		if (ImGui::Begin("Timeline")) {
			if (selectedCount > 0) {
				NodeId nodeId = ids[0];

				if (nodeId.type == NodeType::CLIP) {
					auto& clip = asmData.clips[nodeId.idx];

					float playHeadFrame = 0.0f;
					bool playing{};
					bool currentTimeChanged{};

					ImTimeline::Begin(timelineCtx);
					if (ImTimeline::BeginTimeline("Timeline", &playHeadFrame, 500.0f, 60.0f, &playing, &currentTimeChanged)) {
						for (unsigned int triggerTypeIndex = 0; triggerTypeIndex < asmData.triggerTypeCount; triggerTypeIndex++) {
							if (ImTimeline::BeginTrack(asmData.triggerTypes[triggerTypeIndex])) {
								for (auto& trigger : std::span(asmData.triggers + clip.triggerOffset, clip.triggerCount)) {
									if (trigger.triggerTypeIndex != triggerTypeIndex)
										continue;

									ImGui::PushID(&trigger);
									ImTimeline::Event(trigger.name, &trigger.unknown2);

									if (ImGui::IsItemClicked())
										ImGui::OpenPopup("Editor");

									if (ImGui::BeginPopup("Editor")) {
										Viewer("name", trigger.name);
										Editor("type", reinterpret_cast<uint8_t&>(trigger.type));
										Viewer("trigger type name", trigger.triggerTypeIndex < 0 ? "<none>" : asmData.triggerTypes[trigger.triggerTypeIndex]);
										Viewer("collider", trigger.colliderIndex < 0 ? "<none>" : asmData.colliders[trigger.colliderIndex]);
										Editor("unknown2", trigger.unknown2);
										Editor("unknown3", trigger.unknown3);
										ImGui::EndPopup();
									}
									ImGui::PopID();
								}
								ImTimeline::EndTrack();
							}
						}
						ImTimeline::EndTimeline();
					}
					ImTimeline::End();
				}
			}
		}
		ImGui::End();
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

	void BlendTreeEditor::RenderFlag(short flagId)
	{
		nodeEditor.Flag(flagId);
	}

	void BlendTreeEditor::RenderState(short stateId)
	{
		auto& asmData = *gocAnimator->asmResourceManager->animatorResource->binaryData;
		auto& state = asmData.states[stateId];

		if (state.type == StateType::BLEND_TREE)
			RenderNode(nullptr, state.rootBlendNodeOrClipIndex);

		nodeEditor.State(stateId);

		switch (state.type) {
		case StateType::BLEND_TREE:
			RenderNode(nullptr, state.rootBlendNodeOrClipIndex);
			nodeEditor.StateBlendNode(state.rootBlendNodeOrClipIndex, stateId);
			break;
		case StateType::CLIP:
			nodeEditor.StateClip(state.rootBlendNodeOrClipIndex, stateId);
			break;
		}

		for (unsigned short i = 0; i < state.flagIndexCount; i++)
			nodeEditor.StateFlag(asmData.flagIndices[state.flagIndexOffset + i], stateId);
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

	void BlendTreeEditor::ShowError(const char* msg)
	{
		errMsg = msg;
		ImGui::OpenPopup("ErrorMsg");
	}

	bool BlendTreeEditor::IsType(OutputPinId inPin, InputPinId outPin, NodeType inNodeType, PinType inPinType, NodeType outNodeType, PinType outPinType)
	{
		return inPin.type == inPinType && outPin.type == outPinType && inPin.nodeId.type == inNodeType && outPin.nodeId.type == outNodeType;
	}

	bool BlendTreeEditor::IsType(OutputPinId inPin, InputPinId outPin, NodeType inNodeType, NodeType outNodeType, PinType pinType)
	{
		return IsType(inPin, outPin, inNodeType, pinType, outNodeType, pinType);
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
