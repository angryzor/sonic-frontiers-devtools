#include "Pch.h"
#include "MemoryInspector.h"
#include "imgui/implot_internal.h"

using namespace csl::fnd;
using namespace hh::fnd;

const char* unitNames[4] = { "B", "kiB", "MiB", "GiB" };

void DataSizeText(size_t value, int unit) {
	if (unit == 0)
		ImGui::Text("%zd", value);
	else
		ImGui::Text("%.2f", static_cast<float>(value) / (1 << 10 * unit));
}

HeapInspector::HeapInspector(IAllocator* allocator, MemoryInspector* memoryInspector, HeapBase* target) : ReferencedObject{ allocator, true }, memoryInspector{ memoryInspector }, target { target }
{
}

void HeapInspector::Tick() {
	HeapStatistics stats{};
	target->GetStatistics(stats);

	// We do this separately so that the frames on all inspectors line up even if they aren't being rendered.
	allocationsHistory[nextFrame] = stats.liveAllocations;
	usedHistory[nextFrame] = stats.used;
	nextFrame = (nextFrame + 1) % 512;

	for (auto i = 0; i < childHeapInspectors.size(); i++) {
		bool found{ false };

		for (auto& child : target->GetChildren()) {
			if (childHeapInspectors[i]->target == &child) {
				found = true;
				break;
			}
		}

		if (!found)
			childHeapInspectors.remove(i);
	}

	for (auto& child : target->GetChildren()) {
		bool found{ false };

		for (auto& inspector : childHeapInspectors) {
			if (inspector->target == &child) {
				found = true;
				break;
			}
		}
		
		if (!found)
			childHeapInspectors.push_back(new (GetAllocator()) HeapInspector{ GetAllocator(), memoryInspector, &child });
	}

	for (auto& inspector : childHeapInspectors)
		inspector->Tick();
}

void HeapInspector::Render()
{
	auto* name = target->GetName();

	HeapStatistics stats{};
	target->GetStatistics(stats);

	ImGui::PushID(target);

	ImGui::TableNextRow();
	ImGui::TableNextColumn();

	auto isOpen = ImGui::TreeNodeEx("Node", ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen | (childHeapInspectors.empty() ? ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen : 0), "%s", name);

	ImGui::TableNextColumn();
	DataSizeText(stats.bufferSize, memoryInspector->units);
	ImGui::TableNextColumn();
	DataSizeText(stats.used, memoryInspector->units);
	ImGui::TableNextColumn();
	DataSizeText(stats.allocated, memoryInspector->units);
	ImGui::TableNextColumn();
	ImGui::Text("%d", stats.liveAllocations);
	ImGui::TableNextColumn();
	ImGui::Text("%d", stats.totalAllocations);
	ImGui::TableNextColumn();

	if (ImPlot::BeginPlot("##Allocations", ImVec2(-1, 50), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
		//ImPlot::SetupLegend(ImPlotLocation_SouthWest, ImPlotLegendFlags_Horizontal | ImPlotLegendFlags_Outside);
		ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxis(ImAxis_Y1, "Allocations", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxis(ImAxis_Y2, "Used", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_Opposite);
		if (memoryInspector->absoluteUsed)
			ImPlot::SetupAxisLimits(ImAxis_Y2, 0.0, stats.bufferSize, ImPlotCond_Always);
		else
			ImPlot::GetCurrentContext()->CurrentPlot->Axes[ImAxis_Y2].HasRange = false;
		//ImPlot::SetupAxisLimits(ImAxis_Y2, 0.0, stats.bufferSize, ImPlotCond_Always);
		ImPlot::SetNextLineStyle(ImVec4(0.31f, 0.69f, 0.776f, 1.0f));
		ImPlot::SetNextFillStyle(ImVec4(0.31f, 0.69f, 0.776f, 1.0f), 0.3f);
		ImPlot::SetAxes(ImAxis_X1, ImAxis_Y1);
		ImPlot::PlotLine("Live allocations", allocationsHistory, 512, 1.0, 0.0, ImPlotLineFlags_Shaded, nextFrame);
		ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.643f, 0.231f, 1.0f));
		ImPlot::SetNextFillStyle(ImVec4(0.8f, 0.643f, 0.231f, 1.0f), 0.3f);
		ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
		ImPlot::PlotLine("Used", usedHistory, 512, 1.0, 0.0, ImPlotLineFlags_Shaded, nextFrame);
		ImPlot::EndPlot();
	}

	if (!childHeapInspectors.empty() && isOpen) {
		for (auto& inspector : childHeapInspectors)
			inspector->Render();

		ImGui::TreePop();
	}

	ImGui::PopID();
}

MemoryInspector::MemoryInspector(IAllocator* allocator) : StandaloneWindow{ allocator }
{
	auto* lh = *rangerssdk::GetAddress(&hh::fw::heap::LocalHeap::instance);

	heapInspectors.push_back(new (allocator) HeapInspector{ allocator, this, &lh->baseHeap });

	for (auto& heap : lh->heaps)
		heapInspectors.push_back(new (allocator) HeapInspector{ allocator, this, heap.second });

	SetTitle("Memory Inspector");
}

void MemoryInspector::RenderContents()
{
	for (auto& inspector : heapInspectors)
		inspector->Tick();

	ImGui::SetNextItemWidth(100.0f);
	ImGui::Combo("Units", &units, unitNames, 4);
	ImGui::SameLine();
	ImGui::Checkbox("Scale usage plot to total memory", &absoluteUsed);

	ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));
	if (ImGui::BeginTable("Heap inspectors", 7, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX)) {
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
		ImGui::TableSetupColumn("Total");
		ImGui::TableSetupColumn("Used");
		ImGui::TableSetupColumn("Allocated");
		ImGui::TableSetupColumn("Live allocations");
		ImGui::TableSetupColumn("Total allocations");
		ImGui::TableSetupColumn("Plot (teal = live allocations, yellow = used memory)", ImGuiTableColumnFlags_WidthFixed, 400.0f);
		ImGui::TableHeadersRow();

		for (auto& inspector : heapInspectors)
			inspector->Render();

		ImGui::EndTable();
	}
	ImPlot::PopStyleVar();
}
