#include "Pch.h"
#include "MemoryInspector.h"

using namespace csl::fnd;
using namespace hh::fnd;

AllocatorInspector::AllocatorInspector(IAllocator* allocator, const char* name, ThreadSafeTlsfHeapAllocator* target) : ReferencedObject{ allocator, true }, name{ name }, target { target }
{
}

void AllocatorInspector::Render()
{
	allocationsHistory[nextFrame] = static_cast<float>(target->heap.liveAllocations);
	maxAllocations = std::max(maxAllocations, target->heap.liveAllocations);

	ImGui::SeparatorText(name);
	ImGui::PlotLines("Live allocations", allocationsHistory, 512, nextFrame, nullptr, 0, static_cast<float>(maxAllocations), ImVec2{ 0.0f, 80.0f });
	ImGui::Text("Live allocations: %d", target->heap.liveAllocations);
	ImGui::Text("Total allocations: %d", target->heap.totalAllocations);
	ImGui::Text("Block count: %d", target->heap.blockCount);

	nextFrame = (nextFrame + 1) % 512;
}

MemoryInspector::MemoryInspector(IAllocator* allocator) : StandaloneWindow{ allocator }
{
	auto* ahm = app::fnd::AppHeapManager::GetInstance();

	residentInspector = new (allocator) AllocatorInspector{ allocator, "Resident", &ahm->residentAllocator };
	cyloopInspector = new (allocator) AllocatorInspector{ allocator, "Cyloop", &ahm->cyloopAllocator };
	heightfieldInspector = new (allocator) AllocatorInspector{ allocator, "Heightfield", &ahm->heightfieldAllocator };
	needleInspector = new (allocator) AllocatorInspector{ allocator, "Needle", ahm->needleAllocator };

	SetTitle("Memory Inspector");
}

void MemoryInspector::RenderContents()
{
	residentInspector->Render();
	cyloopInspector->Render();
	heightfieldInspector->Render();
	needleInspector->Render();
}
