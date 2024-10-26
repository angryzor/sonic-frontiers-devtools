#pragma once
#include <ui/common/StandaloneWindow.h>
#include <utilities/CompatibleObject.h>

class MemoryInspector;
class HeapInspector : public CompatibleObject {
	static constexpr size_t numSamples = 256;
	MemoryInspector* memoryInspector;
	uint32_t heapInfoAllocationsHistory[numSamples]{ 0 };
	size_t heapInfoTotalFreeSizeHistory[numSamples]{ 0 };
	size_t heapInfoMaxFreeSizeHistory[numSamples]{ 0 };
	uint32_t statisticsAllocationsHistory[numSamples]{ 0 };
	size_t statisticsAllocatedHistory[numSamples]{ 0 };
	size_t statisticsUsedHistory[numSamples]{ 0 };
	int nextFrame{ 0 };
public:
	csl::ut::MoveArray<hh::fnd::Reference<HeapInspector>> childHeapInspectors{ GetAllocator() };
	csl::fnd::HeapBase* target;
	HeapInspector(csl::fnd::IAllocator* allocator, MemoryInspector* memoryInspector, csl::fnd::HeapBase* target);
	void Tick();
	//void RenderHeapInformation();
	void RenderStatistics();
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

