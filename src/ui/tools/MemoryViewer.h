#pragma once
#include <ui/common/StandaloneWindow.h>
#include <imgui_memory_editor.h>

class MemoryViewer : public StandaloneWindow {
	void* addr{};
	size_t size{};
	MemoryEditor memEditor{};
public:
	MemoryViewer(csl::fnd::IAllocator* allocator, void* addr, size_t size = 0x10000);
	virtual void RenderContents() override;
};
