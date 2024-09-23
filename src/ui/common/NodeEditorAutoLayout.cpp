#include "NodeEditorAutoLayout.h"
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <chrono>

NodeEditorAutoLayout::NodeEditorAutoLayout(csl::fnd::IAllocator* allocator, ax::NodeEditor::EditorContext* context) : CompatibleObject{ allocator }, context{ context }
{
}

NodeEditorAutoLayout::~NodeEditorAutoLayout()
{
	if (recalculationFuture.has_value())
		recalculationFuture->wait();
}

void NodeEditorAutoLayout::Begin()
{
	if (recalculationFuture.has_value()) {
		ImGui::SetNextItemAllowOverlap();
		ImGui::Text("Running auto layout...");
	}
}

void NodeEditorAutoLayout::End()
{
	if (recalculationFuture.has_value() && recalculationFuture->wait_for(std::chrono::seconds::zero()) == std::future_status::ready) {
		auto positions = recalculationFuture->get();
		for (auto it = positions.begin(); it != positions.end(); it++)
			ax::NodeEditor::SetNodePosition(it.key(), *it);

		recalculationFuture = std::nullopt;
		nodes.clear();
		edges.clear();
	}

	if (recalculateRequested) {
		recalculateRequested = false;

		recalculationFuture = std::async([this]() {
			csl::ut::PointerMap<ax::NodeEditor::NodeId, ImVec2> positions{ GetAllocator() };

			{
				ogdf::Graph graph{};
				ogdf::GraphAttributes graphAttrs{ graph, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics };
				csl::ut::PointerMap<ax::NodeEditor::NodeId, ogdf::node> graphNodesByNodeId{ GetAllocator() };

				for (auto node : nodes) {
					ogdf::node n = graph.newNode();
					graphAttrs.width(n) = node.size.y;
					graphAttrs.height(n) = node.size.x;
					graphNodesByNodeId.Insert(node.id, n);
				}

				for (auto edge : edges)
					graph.newEdge(graphNodesByNodeId.GetValueOrFallback(edge.from, nullptr), graphNodesByNodeId.GetValueOrFallback(edge.to, nullptr));

				auto* ohl = new ogdf::OptimalHierarchyLayout{};
				ohl->nodeDistance(50.0);
				ohl->weightBalancing(0.8);

				ogdf::SugiyamaLayout sl{};
				sl.setRanking(new ogdf::OptimalRanking{});
				sl.setCrossMin(new ogdf::MedianHeuristic{});
				sl.setLayout(ohl);
				sl.call(graphAttrs);

				for (auto i = graphNodesByNodeId.begin(); i != graphNodesByNodeId.end(); i++)
					positions.Insert(i.key(), { static_cast<float>(graphAttrs.y(*i)), static_cast<float>(graphAttrs.x(*i)) });
			}

			OGDF_ALLOCATOR::flushPool();

			return positions;
		});
	}
}

void NodeEditorAutoLayout::AddNode(ax::NodeEditor::NodeId id)
{
	if (recalculateRequested)
		nodes.push_back({ id, ax::NodeEditor::GetNodeSize(id) });
}

void NodeEditorAutoLayout::AddLink(ax::NodeEditor::NodeId from, ax::NodeEditor::NodeId to)
{
	if (recalculateRequested)
		edges.push_back({ from, to });
}

void NodeEditorAutoLayout::Recalculate()
{
	if (recalculationFuture.has_value())
		return;

	recalculateRequested = true;
}
