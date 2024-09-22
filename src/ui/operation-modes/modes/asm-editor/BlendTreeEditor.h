#pragma once
#include <ui/common/StandaloneWindow.h>
#include <imgui_node_editor.h>
#include <ui/common/NodeEditorAutoLayout.h>

namespace ui::operation_modes::modes::asm_editor {
	class BlendTreeEditor : public StandaloneWindow {
	public:
		hh::fnd::Reference<hh::anim::GOCAnimator> gocAnimator{};
		hh::fnd::Reference<hh::anim::BlendNodeBase> focusedRootBlendNode{};
		short focusedRootBlendNodeIndex{ -1 };
		ax::NodeEditor::EditorContext* context{};
		NodeEditorAutoLayout autoLayout{ GetAllocator(), context };

		BlendTreeEditor(csl::fnd::IAllocator* allocator, hh::anim::GOCAnimator* gocAnimator, hh::anim::BlendNodeBase* focusedRootBlendNode, short focusedRootBlendNodeIndex);
		virtual ~BlendTreeEditor();
		virtual void RenderContents() override;
		void RenderNodeChildren(hh::anim::BranchBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeChildren(hh::anim::ClipNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeChildren(hh::anim::LayerBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeTitle(hh::anim::BlendNodeBase* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeInputPins(hh::anim::BlendNodeBase* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeInputPins(hh::anim::LayerBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeInputPins(hh::anim::BranchBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeOutputPins(hh::anim::BlendNodeBase* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeLinks(hh::anim::BlendNodeBase* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeLinks(hh::anim::BranchBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeLinks(hh::anim::LayerBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeControls(hh::anim::BlendNodeBase* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeControls(hh::anim::ClipNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		//void RenderNodeControls(hh::anim::LerpBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		//void RenderNodeControls(hh::anim::AdditiveBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		//void RenderNodeControls(hh::anim::ClipNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		//void RenderNodeControls(hh::anim::OverrideBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		//void RenderNodeControls(hh::anim::LayerBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		//void RenderNodeControls(hh::anim::MulBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		void RenderNodeControls(hh::anim::BlendSpaceNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);
		//void RenderNodeControls(hh::anim::TwoPointLerpBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData);

		void RenderNode(hh::anim::BlendNodeBase* node, short nodeId);
		template<typename T> void RenderNode(T* node, short nodeId, hh::anim::BlendNodeData& nodeData) {
			auto& blendNodeData = gocAnimator->asmResourceManager->animatorResource->binaryData->blendNodes[nodeId];

			RenderNodeChildren(node, nodeId, blendNodeData);

			ax::NodeEditor::BeginNode(GetNodeId(nodeId));

			ImGui::PushID(nodeId);
			RenderNodeTitle(node, nodeId, blendNodeData);

			ImGui::BeginGroup();
			RenderNodeInputPins(node, nodeId, blendNodeData);
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			ImGui::PushItemWidth(100.0f);
			RenderNodeControls(node, nodeId, blendNodeData);
			ImGui::PopItemWidth();
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			RenderNodeOutputPins(node, nodeId, blendNodeData);
			ImGui::EndGroup();

			ImGui::PopID();

			ax::NodeEditor::EndNode();
			autoLayout.AddNode(GetNodeId(nodeId));

			RenderNodeLinks(node, nodeId, blendNodeData);
		}

		static ax::NodeEditor::NodeId GetNodeId(short nodeId);
		static ax::NodeEditor::PinId GetInputPinId(short nodeId, unsigned short idx);
		static ax::NodeEditor::PinId GetOutputPinId(short nodeId, unsigned short idx);
		static ax::NodeEditor::LinkId GetLinkId(short fromNodeId, unsigned short fromOutputPinIdx, short toNodeId, unsigned short toOutputPinIdx);
	};
}
