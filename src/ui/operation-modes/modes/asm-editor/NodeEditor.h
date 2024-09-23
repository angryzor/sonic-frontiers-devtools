#pragma once
#include <ui/common/NodeEditor.h>
#include <ranges>

namespace ui::operation_modes::modes::asm_editor {
	enum class NodeType {
		STATE,
		BLEND_NODE,
		VARIABLE,
	};

	enum class PinType {
		DEFAULT_TRANSITION,
		TRANSITION,
		EVENT,
		VARIABLE,
		BLEND_NODE,
		BLEND_MASK,
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
		hh::anim::AsmData& asmData;
		hh::anim::GOCAnimator* gocAnimator;

	public:
		NodeEditor(csl::fnd::IAllocator* allocator, hh::anim::ResAnimator& resource, hh::anim::GOCAnimator* gocAnimator);

		void Begin();
		void End();

		void State(short stateId, ImVec4 color, float progress);
		void StateTransition(short prevStateId, short nextStateId);
		void StateDefaultTransition(short prevStateId, short nextStateId);
		void StateEventTransition(short prevStateId, short nextStateId, unsigned short idx);
		void StateTransitionFlow(short prevStateId, short nextStateId);
		void StateDefaultTransitionFlow(short prevStateId, short nextStateId);
		void StateEventTransitionFlow(short prevStateId, short nextStateId, unsigned short idx);

		void Variable(short variableId);
		void BlendNodeVariable(short variableId, short blendNodeId, unsigned short idx);

		template<typename T>
		void BlendNode(short blendNodeId, T* liveNode) {
			auto nodeId = GetNodeId(NodeType::BLEND_NODE, blendNodeId);
			auto& nodeData = asmData.blendNodes[blendNodeId];

			nodeEditor.BeginNode(nodeId, 10.0f);

			nodeEditor.BeginTitle();
			ImGui::Text(blendNodeNames[static_cast<uint8_t>(nodeData.type)]);
			nodeEditor.EndTitle();

			nodeEditor.BeginInputPins();
			for (unsigned short i = 0; i < nodeData.childNodeArraySize; i++)
				InputPin(nodeId, PinType::BLEND_NODE, i, "Child");
			BlendNodeInputs(blendNodeId, liveNode);
			nodeEditor.EndInputPins();

			nodeEditor.BeginControls();
			ImGui::PushItemWidth(100.0f);
			BlendNodeControls(blendNodeId, liveNode);
			ImGui::PopItemWidth();
			nodeEditor.EndControls();

			nodeEditor.BeginOutputPins();
			OutputPin(nodeId, PinType::BLEND_NODE, 0, "Out");
			nodeEditor.EndOutputPins();

			nodeEditor.EndNode();
		}
		void BlendNodeChildRelationship(short childNodeId, short parentNodeId, short idx);

		bool IsStateSelected(unsigned short idx);

	private:
		static const char* blendNodeNames[8];

		void BlendNodeInputs(short nodeId, hh::anim::BlendNodeBase* node);
		void BlendNodeInputs(short nodeId, hh::anim::BlendSpaceNode* node);
		void BlendNodeControls(short nodeId, hh::anim::BlendNodeBase* node);
		void BlendNodeControls(short nodeId, hh::anim::ClipNode* node);
		//void BlendNodeControls(short nodeId, hh::anim::LerpBlendNode* node);
		//void BlendNodeControls(short nodeId, hh::anim::AdditiveBlendNode* node);
		//void BlendNodeControls(short nodeId, hh::anim::ClipNode* node);
		//void BlendNodeControls(short nodeId, hh::anim::OverrideBlendNode* node);
		//void BlendNodeControls(short nodeId, hh::anim::LayerBlendNode* node);
		//void BlendNodeControls(short nodeId, hh::anim::MulBlendNode* node);
		void BlendNodeControls(short nodeId, hh::anim::BlendSpaceNode* node);
		//void BlendNodeControls(short nodeId, hh::anim::TwoPointLerpBlendNode* node);

		void BeginInputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx);
		void EndInputPin();
		void BeginOutputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx, float labelWidth);
		void EndOutputPin();

		void InputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx, const char* text);
		void InputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx);
		void OutputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx, const char* text);
		void OutputPin(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx);

		static ax::NodeEditor::NodeId GetNodeId(NodeType type, unsigned short idx);
		static ax::NodeEditor::PinId GetPinId(ax::NodeEditor::NodeId nodeId, ax::NodeEditor::PinKind kind, PinType type, unsigned short idx);
		static ax::NodeEditor::PinId GetInputPinId(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx);
		static ax::NodeEditor::PinId GetOutputPinId(ax::NodeEditor::NodeId nodeId, PinType type, unsigned short idx);
		static ax::NodeEditor::LinkId GetLinkId(ax::NodeEditor::PinId fromPinId, ax::NodeEditor::PinId toPinId);
	};
}
