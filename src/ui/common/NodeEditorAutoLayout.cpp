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
	if (recalculateRequested) {
		graph.clear();
		graphAttrs.init(ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);
	}

	if (recalculationFuture.has_value()) {
		ImGui::SetNextItemAllowOverlap();
		ImGui::Text("Running auto layout...");
	}
}

void NodeEditorAutoLayout::End()
{
	if (recalculationFuture.has_value() && recalculationFuture->wait_for(std::chrono::seconds::zero()) == std::future_status::ready) {
		for (auto it = autoLayout.begin(); it != autoLayout.end(); it++) {
			ogdf::node n = *it;

			ax::NodeEditor::SetNodePosition(it.key(), { static_cast<float>(graphAttrs.y(n)), static_cast<float>(graphAttrs.x(n)) });
		}
		recalculationFuture = std::nullopt;
	}

	if (recalculateRequested) {
		recalculateRequested = false;

		recalculationFuture = std::async([this]() {
			auto* ohl = new ogdf::OptimalHierarchyLayout{};
			ohl->nodeDistance(50.0);
			ohl->weightBalancing(0.8);

			ogdf::SugiyamaLayout sl{};
			sl.setRanking(new ogdf::OptimalRanking{});
			sl.setCrossMin(new ogdf::MedianHeuristic{});
			sl.setLayout(ohl);
			sl.call(graphAttrs);
		});
	}
}

void NodeEditorAutoLayout::AddNode(ax::NodeEditor::NodeId id)
{
	if (recalculateRequested) {
		auto size = ImGui::GetItemRectSize();
		ogdf::node n = graph.newNode();
		graphAttrs.width(n) = size.y;
		graphAttrs.height(n) = size.x;

		autoLayout.Insert(id, n);
	}
}

void NodeEditorAutoLayout::AddLink(ax::NodeEditor::NodeId from, ax::NodeEditor::NodeId to)
{
	if (recalculateRequested)
		graph.newEdge(autoLayout.GetValueOrFallback(from, nullptr), autoLayout.GetValueOrFallback(to, nullptr));
}

void NodeEditorAutoLayout::Recalculate()
{
	if (recalculationFuture.has_value())
		return;

	recalculateRequested = true;
}
