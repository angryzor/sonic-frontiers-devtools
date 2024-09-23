#pragma once
#include <utilities/CompatibleObject.h>
#include <imgui_node_editor.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <optional>
#include <future>

class NodeEditorAutoLayout : public CompatibleObject {
private:
	struct Node {
		ax::NodeEditor::NodeId id{};
		ImVec2 size{};
	};

	struct Edge {
		ax::NodeEditor::NodeId from{};
		ax::NodeEditor::NodeId to{};
	};

	csl::ut::MoveArray<Node> nodes{ GetAllocator() };
	csl::ut::MoveArray<Edge> edges{ GetAllocator() };
	bool recalculateRequested{ true };
	bool recalculationCompleted{ false };
	std::optional<std::future<csl::ut::PointerMap<ax::NodeEditor::NodeId, ImVec2>>> recalculationFuture{};

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
