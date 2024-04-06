#pragma once
#include "StandaloneWindow.h"

class MemoryInspector;
class HeapInspector : public hh::fnd::ReferencedObject {
	MemoryInspector* memoryInspector;
	uint32_t allocationsHistory[512]{ 0 };
	size_t allocatedHistory[512]{ 0 };
	size_t usedHistory[512]{ 0 };
	int nextFrame{ 0 };
	csl::ut::MoveArray<hh::fnd::Reference<HeapInspector>> childHeapInspectors{ GetAllocator() };
public:
	csl::fnd::HeapBase* target;
	HeapInspector(csl::fnd::IAllocator* allocator, MemoryInspector* memoryInspector, csl::fnd::HeapBase* target);
	void Tick();
	void Render();
};

class MemoryInspector : public StandaloneWindow {
	csl::ut::MoveArray<hh::fnd::Reference<HeapInspector>> heapInspectors{ GetAllocator() };
public:
	int units{ 0 };
	MemoryInspector(csl::fnd::IAllocator* allocator);
	virtual void PreRender() override;
	virtual void RenderContents() override;
};

