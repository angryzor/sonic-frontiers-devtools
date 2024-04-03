#pragma once
#include "StandaloneWindow.h"

class AllocatorInspector : public hh::fnd::ReferencedObject {
	const char* name;
	hh::fnd::ThreadSafeTlsfHeapAllocator* target;
	float allocationsHistory[512]{ 0 };
	uint32_t maxAllocations{ 0 };
	int nextFrame{ 0 };
public:
	AllocatorInspector(csl::fnd::IAllocator* allocator, const char* name, hh::fnd::ThreadSafeTlsfHeapAllocator* target);
	void Render();
};

class MemoryInspector : public StandaloneWindow {
	hh::fnd::Reference<AllocatorInspector> residentInspector;
	hh::fnd::Reference<AllocatorInspector> cyloopInspector;
	hh::fnd::Reference<AllocatorInspector> heightfieldInspector;
	hh::fnd::Reference<AllocatorInspector> needleInspector;
public:
	MemoryInspector(csl::fnd::IAllocator* allocator);
	virtual void RenderContents();
};

