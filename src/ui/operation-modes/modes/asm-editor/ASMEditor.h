#pragma once
#include <imgui_node_editor.h>
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

namespace ui::operation_modes::modes::asm_editor
{
	class ASMEditor : public OperationMode<Context>
	{
		ax::NodeEditor::EditorContext* edContext{};
		ogdf::Graph autoLayoutGraph{};
		ogdf::GraphAttributes autoLayoutGraphAttributes{ autoLayoutGraph, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics };
		csl::ut::MoveArray<ogdf::node> autoLayout{ GetAllocator() };
		bool autoLayouted{ false };

	public:
		ASMEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host);
		virtual ~ASMEditor();
		virtual void RenderScene() override;

	private:
		static ax::NodeEditor::NodeId GetStateId(short stateId);
		static ax::NodeEditor::PinId GetInputPinId(short stateId);
		static ax::NodeEditor::PinId GetOutputPinId(short stateId);
		static ax::NodeEditor::LinkId GetLinkId(short fromStateId, short toStateId);
	};
}
