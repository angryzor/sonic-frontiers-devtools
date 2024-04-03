#pragma once
#include "StandaloneWindow.h"

class HeapInspector : public hh::fnd::ReferencedObject {
	const char* name;
	csl::fnd::TlsfHeapBase* target;
	float allocationsHistory[512]{ 0 };
	uint32_t maxAllocations{ 0 };
	int nextFrame{ 0 };
public:
	HeapInspector(csl::fnd::IAllocator* allocator, const char* name, csl::fnd::TlsfHeapBase* target);
	void Render();
};

class MemoryInspector : public StandaloneWindow {
	hh::fnd::Reference<HeapInspector> moduleInspector;
	hh::fnd::Reference<HeapInspector> debugInspector;
	hh::fnd::Reference<HeapInspector> criHedgehogInspector;
	hh::fnd::Reference<HeapInspector> residentInspector;
	hh::fnd::Reference<HeapInspector> cyloopInspector;
	hh::fnd::Reference<HeapInspector> heightfieldInspector;
	hh::fnd::Reference<HeapInspector> needleInspector;
public:
	MemoryInspector(csl::fnd::IAllocator* allocator);
	virtual void RenderContents();
};

