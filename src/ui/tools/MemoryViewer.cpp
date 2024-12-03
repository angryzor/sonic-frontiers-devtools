#include "MemoryViewer.h"

MemoryViewer::MemoryViewer(csl::fnd::IAllocator* allocator, void* addr, size_t size) : StandaloneWindow{ allocator }, addr{ addr }, size{ size } {
	char namebuf[500];
	snprintf(namebuf, sizeof(namebuf), "Memory viewer @ 0x%zx - viewing %zx bytes", addr, size);
	SetTitle(namebuf);
}

void MemoryViewer::RenderContents() {
	memEditor.DrawContents(addr, size);
}
