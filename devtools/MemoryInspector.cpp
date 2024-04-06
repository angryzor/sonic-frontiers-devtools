#include "Pch.h"
#include "MemoryInspector.h"

using namespace csl::fnd;
using namespace hh::fnd;

const char* unitNames[4] = { "B", "kiB", "MiB", "GiB" };

void DataSizeText(size_t value, size_t total, int unit) {
	if (unit == 0)
		ImGui::Text("%zd (%d%%)", value, value * 100 / total);
	else
		ImGui::Text("%.2f (%d%%)", static_cast<float>(value) / (1 << 10 * unit), value * 100 / total);
}

HeapInspector::HeapInspector(IAllocator* allocator, MemoryInspector* memoryInspector, HeapBase* target) : ReferencedObject{ allocator, true }, memoryInspector{ memoryInspector }, target { target }
{
}

void HeapInspector::Tick() {
	HeapStatistics stats{};
	target->GetStatistics(stats);

	// We do this separately so that the frames on all inspectors line up even if they aren't being rendered.
	allocationsHistory[nextFrame] = stats.liveAllocations;
	allocatedHistory[nextFrame] = stats.allocated;
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
	DataSizeText(stats.bufferSize, stats.bufferSize, memoryInspector->units);
	ImGui::TableNextColumn();
	DataSizeText(stats.used, stats.bufferSize, memoryInspector->units);
	ImGui::TableNextColumn();
	DataSizeText(stats.allocated, stats.bufferSize, memoryInspector->units);
	ImGui::TableNextColumn();
	ImGui::Text("%d", stats.liveAllocations);
	ImGui::TableNextColumn();
	ImGui::Text("%d", stats.totalAllocations);
	ImGui::TableNextColumn();

	if (ImPlot::BeginPlot("##Usage", ImVec2(-1, 50), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
		//ImPlot::SetupLegend(ImPlotLocation_SouthWest, ImPlotLegendFlags_Horizontal | ImPlotLegendFlags_Outside);
		ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxis(ImAxis_Y1, "Memory size", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, static_cast<double>(stats.bufferSize), ImPlotCond_Always);
		ImPlot::SetNextLineStyle(ImVec4(0.471f, 0.349f, 0.392f, 1.0f));
		ImPlot::SetNextFillStyle(ImVec4(0.471f, 0.349f, 0.392f, 1.0f));
		ImPlot::PlotLine("Allocated", allocatedHistory, 512, 1.0, 0.0, ImPlotLineFlags_Shaded, nextFrame);
		ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.643f, 0.231f, 1.0f));
		ImPlot::SetNextFillStyle(ImVec4(0.8f, 0.643f, 0.231f, 1.0f));
		ImPlot::PlotLine("Used", usedHistory, 512, 1.0, 0.0, ImPlotLineFlags_Shaded, nextFrame);
		ImPlot::EndPlot();
	}
	ImGui::TableNextColumn();

	if (ImPlot::BeginPlot("##Activity", ImVec2(-1, 50), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
		//ImPlot::SetupLegend(ImPlotLocation_SouthWest, ImPlotLegendFlags_Horizontal | ImPlotLegendFlags_Outside);
		ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxis(ImAxis_Y1, "Allocations", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
		ImPlot::SetupAxis(ImAxis_Y2, "Used", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_Opposite);
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

void MemoryInspector::PreRender() {
	ImGui::SetNextWindowSize(ImVec2(1600, 1100), ImGuiCond_Once);
}

void MemoryInspector::RenderContents()
{
	for (auto& inspector : heapInspectors)
		inspector->Tick();

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Units")) {
			for (int i = 0; i < 4; i++)
				if (ImGui::MenuItem(unitNames[i], nullptr, units == i))
					units = i;
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	char totalHeader[20]{};
	char usedHeader[20]{};
	char allocatedHeader[20]{};

	snprintf(totalHeader, sizeof(totalHeader), "Total (%s)", unitNames[units]);
	snprintf(usedHeader, sizeof(usedHeader), "Used (%s)", unitNames[units]);
	snprintf(allocatedHeader, sizeof(allocatedHeader), "Allocated (%s)", unitNames[units]);

	ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));
	if (ImGui::BeginTable("Heap inspectors", 8, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX)) {
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthFixed, 200.0f);
		ImGui::TableSetupColumn(totalHeader);
		ImGui::TableSetupColumn(usedHeader);
		ImGui::TableSetupColumn(allocatedHeader);
		ImGui::TableSetupColumn("Live allocations");
		ImGui::TableSetupColumn("Total allocations");
		ImGui::TableSetupColumn("Usage plot (teal = allocated unused memory, yellow = used memory)", ImGuiTableColumnFlags_WidthFixed, 400.0f);
		ImGui::TableSetupColumn("Activity plot (teal = live allocations, yellow = used memory)", ImGuiTableColumnFlags_WidthFixed, 400.0f);
		ImGui::TableHeadersRow();

		for (auto& inspector : heapInspectors)
			inspector->Render();

		ImGui::EndTable();
	}
	ImPlot::PopStyleVar();
}
