#pragma once
#include <ui/common/NodeEditor.h>
#include <ranges>
#include <imgui_internal.h>

namespace ui::operation_modes::modes::asm_editor {
	enum class NodeType {
		STATE,
		BLEND_NODE,
		VARIABLE,
		CLIP,
		LAYER_BLEND_TREE_OUTPUT,
		BLEND_SPACE,
		BLEND_MASK,
	};

	enum class PinType {
		DEFAULT_TRANSITION,
		TRANSITION,
		EVENT,
		VARIABLE,
		BLEND_NODE,
		BLEND_MASK,
		CLIP,
		BLEND_SPACE,
	};

	//struct NodeId {
	//	NodeType type;
	//	unsigned short id;

	//	operator ax::NodeEditor::NodeId();
	//};
	//
	//struct PinId {
	//	NodeId nodeId;
	//	PinType type;
	//	unsigned short id;

	//	operator ax::NodeEditor::PinId();
	//};

	class NodeEditorInterface {
	public:
		using PinType = PinType;
		static void RenderPinIcon(PinType type);
		static ImVec4 GetColor(PinType type);
	};

	class RawNodeEditor : public ::NodeEditor<NodeEditorInterface> {
	public:
		using ::NodeEditor<NodeEditorInterface>::NodeEditor;
	};

	class NodeEditor : public CompatibleObject {
		RawNodeEditor nodeEditor;
		hh::anim::ResAnimator& resource;
		hh::anim::AsmData& asmData;
		hh::anim::GOCAnimator* gocAnimator;

	public:
		NodeEditor(csl::fnd::IAllocator* allocator, hh::anim::ResAnimator& resource, hh::anim::GOCAnimator* gocAnimator);

		void Begin();
		void End();

		void RunAutoLayout();

		void State(short stateId, ImVec4 color, float progress);
		void StateTransition(short prevStateId, short nextStateId);
		void StateDefaultTransition(short prevStateId, short nextStateId);
		void StateEventTransition(short prevStateId, short nextStateId, unsigned short idx);
		void StateTransitionFlow(short prevStateId, short nextStateId);
		void StateDefaultTransitionFlow(short prevStateId, short nextStateId);
		void StateEventTransitionFlow(short prevStateId, short nextStateId, unsigned short idx);
		void StateClip(short clipId, short stateId);
		void StateBlendNode(short blendNodeId, short stateId);

		void Variable(short variableId);
		void BlendNodeVariable(short variableId, short blendNodeId, unsigned short idx);
		void BlendSpaceVariable(short variableId, short blendSpaceId, unsigned short idx);

		void Clip(short clipId);
		void BlendNodeClip(short clipId, short blendNodeId, unsigned short idx);
		void BlendSpaceClip(short clipId, short blendSpaceId, unsigned short idx);

		void BlendSpace(short blendSpaceId);
		void BlendNodeBlendSpace(short blendSpaceId, short blendNodeId);

		void BlendMask(short blendMaskId);
		void ClipBlendMask(short blendMaskId, short clipId);

		void LerpBlendNode(short blendNodeId, hh::anim::LerpBlendNode* liveNode);
		void AdditiveBlendNode(short blendNodeId, hh::anim::AdditiveBlendNode* liveNode);
		void ClipNode(short blendNodeId, hh::anim::ClipNode* liveNode);
		void OverrideBlendNode(short blendNodeId, hh::anim::OverrideBlendNode* liveNode);
		void LayerBlendNode(short blendNodeId, hh::anim::LayerBlendNode* liveNode);
		void MulBlendNode(short blendNodeId, hh::anim::MulBlendNode* liveNode);
		void BlendSpaceNode(short blendNodeId, hh::anim::BlendSpaceNode* liveNode);
		void CollapsedBlendSpaceNode(short blendNodeId, hh::anim::BlendSpaceNode* liveNode);
		void TwoPointLerpBlendNode(short blendNodeId, hh::anim::TwoPointLerpBlendNode* liveNode);
		void BlendNodeChildRelationship(short childNodeId, short parentNodeId, short idx);

		void LayerBlendTreeOutput(short blendNodeId);

		bool IsStateSelected(unsigned short idx);

	private:
		template<typename I, typename C>
		void BlendNode(short blendNodeId, const char* name, I renderInputPins, C renderControls) {
			auto nodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
			auto& nodeData = asmData.blendNodes[blendNodeId];

			nodeEditor.BeginNode(nodeId, 0.0f);

			nodeEditor.BeginTitle();
			ImGui::Text("Blend node - %s", name);
			nodeEditor.EndTitle();

			nodeEditor.BeginInputPins();
			renderInputPins();
			nodeEditor.EndInputPins();

			nodeEditor.BeginControls();
			ImGui::PushItemWidth(100.0f);
			renderControls();
			ImGui::PopItemWidth();
			nodeEditor.EndControls();

			nodeEditor.BeginOutputPins();
			OutputPin(nodeId, PinType::BLEND_NODE, 0);
			nodeEditor.EndOutputPins();

			nodeEditor.EndNode();
		}
		template<typename C>
		void BranchBlendNode(short blendNodeId, const char* name, C renderControls) {
			BlendNode(
				blendNodeId,
				name,
				[=]() {
					auto nodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
					auto& nodeData = asmData.blendNodes[blendNodeId];

					BaseBlendNodeInputs(blendNodeId);

					for (unsigned short i = 0; i < nodeData.childNodeArraySize; i++)
						InputPin(nodeId, PinType::BLEND_NODE, i, "Child");
				},
				renderControls
			);
		}
		void SimpleBlendNode(short blendNodeId, const char* name);
		void SimpleBranchBlendNode(short blendNodeId, const char* name);

		void BaseBlendNodeInputs(short blendNodeId);
		void BaseBlendNodeControls(short blendNodeId);

		void BeginInputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx);
		void EndInputPin();
		void BeginOutputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx, float labelWidth);
		void EndOutputPin();

		template<typename... Args>
		void InputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx, const char* fmt, Args... args) {
			BeginInputPin(nodeId, type, idx);
			ImGui::Text("%s", fmt, args...);
			EndInputPin();
		}

		template<typename... Args>
		void OutputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx, const char* fmt, Args... args) {
			const char *text, *text_end;
			ImFormatStringToTempBuffer(&text, &text_end, fmt, args...);
			BeginOutputPin(nodeId, type, idx, ImGui::CalcTextSize(text).x + ImGui::GetStyle().ItemSpacing.x);
			ImGui::Text("%s", text);
			EndOutputPin();
		}

		template<typename... Args>
		void InputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx, const char* label, const char* fmt, Args... args) {
			BeginInputPin(nodeId, type, idx);
			ImGui::LabelText(label, fmt, args...);
			EndInputPin();
		}
		template<typename... Args>
		void OutputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx, const char* label, const char* fmt, Args... args) {
			const char *text, *text_end;
			ImFormatStringToTempBuffer(&text, &text_end, fmt, args...);
			BeginOutputPin(nodeId, type, idx, ImGui::CalcTextSize(label).x + ImGui::GetStyle().ItemInnerSpacing.x + ImGui::CalcTextSize(text).x + ImGui::GetStyle().ItemSpacing.x);
			ImGui::LabelText(label, "%s", fmt);
			EndOutputPin();
		}

		void InputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx);
		void OutputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx);

		void InputVariablePin(ax::NodeEditor::NodeId nodeId, unsigned short idx, const char* label, short variableId);
		void InputClipPin(ax::NodeEditor::NodeId nodeId, unsigned short idx, const char* label, short clipId);
		void InputBlendMaskPin(ax::NodeEditor::NodeId nodeId, unsigned short idx, const char* label, short blendMaskId);

		void BlendSpaceVariablePins(ax::NodeEditor::NodeId nodeId, short blendSpaceId, unsigned short startIdx);
		void BlendSpaceControls(short blendSpaceId);

		static ax::NodeEditor::NodeId GetNodeId(NodeType type, unsigned short idx);
		static ax::NodeEditor::PinId GetPinId(ax::NodeEditor::NodeId nodeId, ax::NodeEditor::PinKind kind, PinType type, unsigned short idx);
		static ax::NodeEditor::PinId GetInputPinId(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx);
		static ax::NodeEditor::PinId GetOutputPinId(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx);
		static ax::NodeEditor::LinkId GetLinkId(ax::NodeEditor::PinId fromPinId, ax::NodeEditor::PinId toPinId);
	};
}
