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
		FLAG,
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
		FLAG,
	};

	struct NodeId {
		NodeType type{};
		short idx{};

		NodeId() = default;
		NodeId(NodeType type, short idx);
		NodeId(unsigned long long nodeId);
		NodeId(ax::NodeEditor::NodeId nodeId);
		operator unsigned long long() const;
		operator ax::NodeEditor::NodeId() const;
	};

	struct PinId {
		NodeId nodeId{};
		ax::NodeEditor::PinKind kind{};
		PinType type{};
		unsigned short idx{};

		PinId() = default;
		PinId(const NodeId& nodeId, ax::NodeEditor::PinKind kind, PinType type, unsigned short idx);
		PinId(unsigned long long nodeId);
		PinId(ax::NodeEditor::PinId nodeId);
		operator unsigned long long() const;
		operator ax::NodeEditor::PinId() const;
	};

	struct InputPinId : PinId {
		InputPinId() = default;
		InputPinId(const NodeId& nodeId, PinType type, unsigned short idx);
		InputPinId(unsigned long long nodeId);
		InputPinId(ax::NodeEditor::PinId nodeId);
	};

	struct OutputPinId : PinId {
		OutputPinId() = default;
		OutputPinId(const NodeId& nodeId, PinType type, unsigned short idx);
		OutputPinId(unsigned long long nodeId);
		OutputPinId(ax::NodeEditor::PinId nodeId);
	};

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

		void State(short stateId);
		void StateTransition(short prevStateId, short nextStateId);
		void StateDefaultTransition(short prevStateId, short nextStateId);
		void StateEventTransition(short prevStateId, short nextStateId, unsigned short idx);
		void StateTransitionFlow(short prevStateId, short nextStateId);
		void StateDefaultTransitionFlow(short prevStateId, short nextStateId);
		void StateEventTransitionFlow(short prevStateId, short nextStateId, unsigned short idx);
		void StateTransitionFlowAuto(short prevStateId, short nextStateId);
		void StateActiveTransitionFlow();
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
		void BlendNodeChildRelationship(short childNodeId, short parentNodeId, unsigned short idx);

		void Flag(short flagId);
		void StateFlag(short flagId, short stateId);

		void LayerBlendTreeOutput(short blendNodeId);

		bool IsStateSelected(short idx);

		bool ShowNodeContextMenu(NodeId& nodeId);
		bool ShowPinContextMenu(PinId& pinId);

		bool BeginCreate();
		bool QueryNewLink(OutputPinId& startPinId, InputPinId& endPinId);
		void EndCreate();

	private:
		template<typename I, typename C>
		void BlendNode(short blendNodeId, const char* name, I renderInputPins, C renderControls) {
			NodeId nodeId{ NodeType::BLEND_NODE, blendNodeId };

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
			OutputPin({ nodeId, PinType::BLEND_NODE, 0 });
			nodeEditor.EndOutputPins();

			nodeEditor.EndNode();
		}
		template<typename C>
		void BranchBlendNode(short blendNodeId, const char* name, C renderControls) {
			BlendNode(
				blendNodeId,
				name,
				[=]() {
					NodeId nodeId{ NodeType::BLEND_NODE, blendNodeId };
					auto& nodeData = asmData.blendNodes[blendNodeId];

					BaseBlendNodeInputs(blendNodeId);

					for (unsigned short i = 0; i < nodeData.childNodeArraySize; i++)
						InputPin({ nodeId, PinType::BLEND_NODE, i }, "Child");
				},
				renderControls
			);
		}
		void SimpleBlendNode(short blendNodeId, const char* name);
		void SimpleBranchBlendNode(short blendNodeId, const char* name);

		void BaseBlendNodeInputs(short blendNodeId);
		void BaseBlendNodeControls(short blendNodeId);

		struct ActiveLayerInfo {
			hh::anim::AnimationStateMachine::LayerInfo& layer;
			hh::anim::AnimationState* prevState;
			hh::anim::AnimationState* nextState;
		};

		auto GetActiveLayers() {
			return std::views::all(gocAnimator->animationStateMachine->layers)
				| std::views::filter([](auto& l) { return l.layerState != nullptr; })
				| std::views::transform([](auto& l) { return ActiveLayerInfo{ l, l.layerState->GetPreviousAnimationState(), l.layerState->GetNextAnimationState() }; });
		}

		ImVec4 CalculateActiveStateColor(short stateId);
		float CalculateActiveStateProgress(short stateId);

		void BeginInputPin(const InputPinId& pinId);
		void EndInputPin();
		void BeginOutputPin(const OutputPinId& pinId, float labelWidth);
		void EndOutputPin();

		template<typename... Args>
		void InputPin(const InputPinId& pinId, const char* fmt, Args... args) {
			BeginInputPin(pinId);
			ImGui::Text("%s", fmt, args...);
			EndInputPin();
		}

		template<typename... Args>
		void OutputPin(const OutputPinId& pinId, const char* fmt, Args... args) {
			const char* text, * text_end;
			ImFormatStringToTempBuffer(&text, &text_end, fmt, args...);
			BeginOutputPin(pinId, ImGui::CalcTextSize(text).x + ImGui::GetStyle().ItemSpacing.x);
			ImGui::Text("%s", text);
			EndOutputPin();
		}

		template<typename... Args>
		void InputPin(const InputPinId& pinId, const char* label, const char* fmt, Args... args) {
			BeginInputPin(pinId);
			ImGui::LabelText(label, fmt, args...);
			EndInputPin();
		}
		template<typename... Args>
		void OutputPin(const OutputPinId& pinId, const char* label, const char* fmt, Args... args) {
			const char* text, * text_end;
			ImFormatStringToTempBuffer(&text, &text_end, fmt, args...);
			BeginOutputPin(pinId, ImGui::CalcTextSize(label).x + ImGui::GetStyle().ItemInnerSpacing.x + ImGui::CalcTextSize(text).x + ImGui::GetStyle().ItemSpacing.x);
			ImGui::LabelText(label, "%s", fmt);
			EndOutputPin();
		}

		void InputPin(const InputPinId& pinId);
		void OutputPin(const OutputPinId& pinId);

		void InputVariablePin(const NodeId& nodeId, unsigned short idx, const char* label, short variableId);
		void InputClipPin(const NodeId& nodeId, unsigned short idx, const char* label, short clipId);
		void InputBlendMaskPin(const NodeId& nodeId, unsigned short idx, const char* label, short blendMaskId);

		void Link(const OutputPinId& fromPin, const InputPinId& toPin);
		void LayoutLink(const NodeId& fromNode, const NodeId& toNode);
		void LinkWithLayout(const OutputPinId& fromPin, const InputPinId& toPin);
		void Flow(const OutputPinId& fromPin, const InputPinId& toPin);

		//void Link(NodeType fromNodeType, unsigned short fromNodeIdx, PinType fromPinType, unsigned short fromPinIdx, NodeType toNodeType, unsigned short toNodeIdx, PinType toPinType, unsigned short toPinIdx);
		//void LayoutLink(NodeType fromNodeType, unsigned short fromNodeIdx, NodeType toNodeType, unsigned short toNodeIdx);
		//void Flow(NodeType fromNodeType, unsigned short fromNodeIdx, PinType fromPinType, unsigned short fromPinIdx, NodeType toNodeType, unsigned short toNodeIdx, PinType toPinType, unsigned short toPinIdx);

		void BlendSpaceVariablePins(ax::NodeEditor::NodeId nodeId, short blendSpaceId, unsigned short startIdx);
		void BlendSpaceControls(short blendSpaceId);

		//static ax::NodeEditor::NodeId GetNodeId(NodeType type, unsigned short idx);
		//static ax::NodeEditor::PinId GetPinId(ax::NodeEditor::NodeId nodeId, ax::NodeEditor::PinKind kind, PinType type, unsigned short idx);
		//static ax::NodeEditor::PinId GetInputPinId(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx);
		//static ax::NodeEditor::PinId GetOutputPinId(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx);
		static ax::NodeEditor::LinkId GetLinkId(ax::NodeEditor::PinId fromPinId, ax::NodeEditor::PinId toPinId);

		//static void DecodeNodeId(ax::NodeEditor::NodeId nodeId, NodeType& type, unsigned short& idx);
		//static void DecodePinId(ax::NodeEditor::PinId pinId, ax::NodeEditor::PinKind kind, PinType type, unsigned short idx);
		//static void DecodeInputPinId(ax::NodeEditor::PinId pinId, PinType type, unsigned short idx);
		//static void DecodeOutputPinId(ax::NodeEditor::PinId pinId, PinType type, unsigned short idx);
		//static ax::NodeEditor::LinkId DecodeLinkId(ax::NodeEditor::PinId fromPinId, ax::NodeEditor::PinId toPinId);
	};
}
