#include "DebugRenderingSystem.h"
#include "DebugRenderer.h"

namespace devtools::debug_rendering {
	hh::fnd::Reference<DebugRenderingSystem> DebugRenderingSystem::instance{};

	void DebugRenderingSystem::Init(csl::fnd::IAllocator* allocator) {
		DebugRenderer::Init(allocator);
		instance = new (allocator) DebugRenderingSystem{ allocator };
	}

	void DebugRenderingSystem::Deinit() {
		instance = nullptr;
		DebugRenderer::Deinit();
	}
}
