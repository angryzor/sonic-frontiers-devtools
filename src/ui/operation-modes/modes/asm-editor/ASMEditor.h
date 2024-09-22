#pragma once
#include <imgui_node_editor.h>
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"
//#include <ogdf/basic/Graph.h>
//#include <ogdf/basic/GraphAttributes.h>
#include <ui/common/NodeEditorAutoLayout.h>

namespace ui::operation_modes::modes::asm_editor
{
	class ASMEditor : public OperationMode<Context>
	{
		ax::NodeEditor::EditorContext* edContext{};
		NodeEditorAutoLayout autoLayout{ GetAllocator(), edContext};
		//ogdf::Graph autoLayoutGraph{};
		//ogdf::GraphAttributes autoLayoutGraphAttributes{ autoLayoutGraph, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics };
		//csl::ut::MoveArray<ogdf::node> autoLayout{ GetAllocator() };
		//bool autoLayouted{ false };

	public:
		ASMEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host);
		virtual ~ASMEditor();
		virtual void RenderScene() override;

	private:
		enum class PinType : unsigned char {
			INPUT,
			DEFAULT_TRANSITION,
			TRANSITION,
			EVENT,
		};

		static ax::NodeEditor::NodeId GetNodeId(short stateId);
		static ax::NodeEditor::PinId GetPinId(short stateId, PinType type, unsigned short idx);
		static ax::NodeEditor::PinId GetInputPinId(short stateId);
		static ax::NodeEditor::PinId GetDefaultTransitionPinId(short stateId);
		static ax::NodeEditor::PinId GetTransitionPinId(short stateId);
		static ax::NodeEditor::PinId GetEventPinId(short stateId, unsigned short idx);
		static ax::NodeEditor::LinkId GetLinkId(ax::NodeEditor::PinId fromPinId, short toState);
		ax::NodeEditor::PinId ResolveOutputPin(short prevStateId, short nextStateId);
		static void RenderInputPin(short stateId);
		static void RenderDefaultTransitionPin(short stateId, float textWidth);
		static void RenderTransitionPin(short stateId, float textWidth);
		static void RenderEventPin(short stateId, short eventId, hh::anim::EventData& event, float textWidth);
		static void RenderLink(ax::NodeEditor::PinId sourcePin, short targetStateId, const ImVec4& color);
	};
}
