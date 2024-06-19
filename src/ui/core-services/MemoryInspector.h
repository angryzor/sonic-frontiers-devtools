#pragma once
#include <ui/common/StandaloneWindow.h>

class MemoryInspector;
class HeapInspector : public hh::fnd::ReferencedObject {
	static constexpr size_t numSamples = 256;
	MemoryInspector* memoryInspector;
	uint32_t allocationsHistory[numSamples]{ 0 };
	size_t allocatedHistory[numSamples]{ 0 };
	size_t usedHistory[numSamples]{ 0 };
	int nextFrame{ 0 };
public:
	csl::ut::MoveArray<hh::fnd::Reference<HeapInspector>> childHeapInspectors{ GetAllocator() };
	csl::fnd::HeapBase* target;
	HeapInspector(csl::fnd::IAllocator* allocator, MemoryInspector* memoryInspector, csl::fnd::HeapBase* target);
	void Tick();
	void Render();
	void RenderAllocationMaps();
	void RenderDetails();
};

class MemoryInspector : public StandaloneWindow {
	csl::ut::MoveArray<hh::fnd::Reference<HeapInspector>> heapInspectors{ GetAllocator() };
	HeapInspector* detailsInspector{};
public:
	int units{ 0 };
	MemoryInspector(csl::fnd::IAllocator* allocator);
	virtual void PreRender() override;
	virtual void RenderContents() override;
	void RenderHeapSelectComboItem(HeapInspector* inspector);
};

