#pragma once
#include <utilities/CompatibleObject.h>
#include <imgui_node_editor.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <optional>
#include <future>

class NodeEditorAutoLayout : public CompatibleObject {
private:
	ogdf::Graph graph{};
	ogdf::GraphAttributes graphAttrs{ graph, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics };
	csl::ut::PointerMap<ax::NodeEditor::NodeId, ogdf::node> autoLayout{ GetAllocator() };
	bool recalculateRequested{ true };
	bool recalculationCompleted{ false };
	std::optional<std::future<void>> recalculationFuture{};

public:
	ax::NodeEditor::EditorContext* context{};

	NodeEditorAutoLayout(csl::fnd::IAllocator* allocator, ax::NodeEditor::EditorContext* context);
	~NodeEditorAutoLayout();
	void Begin();
	void End();
	void AddNode(ax::NodeEditor::NodeId id);
	void AddLink(ax::NodeEditor::NodeId from, ax::NodeEditor::NodeId to);
	void Recalculate();
};
