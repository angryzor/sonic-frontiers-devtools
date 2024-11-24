#pragma once
#include <tuple>

namespace devtools::target_config {
	namespace addresses {
		constexpr size_t appResetAddr = 0x1501A41F0;
		constexpr size_t appShutdownAddr = 0x150192E80;
		constexpr size_t wndProcAddr = 0x140D68F80;
		constexpr size_t displaySwapDeviceResizeBuffersAddr = 0x1410FB090;
		constexpr size_t displaySwapDevicePresentAddr = 0x1410FAEE0;
		constexpr size_t gocVisualDebugDrawSetup = 0x140D06320;
	}

	namespace memory {
		using devtools_allocator_type = hh::fnd::TlsfHeapAllocator;
	}

	namespace debug_rendering {
		using default_renderables = std::tuple<
		>;
	}
}

#define DEVTOOLS_FEATURE_DEVTOOLS_HEAP
#define DEVTOOLS_FEATURE_PASS_ALLOCATOR
#define DEVTOOLS_FEATURE_CONFIGURABLE_COMPONENT_DATA
#define DEVTOOLS_FEATURE_TERRAIN_MANAGER
#define DEVTOOLS_FEATURE_FREE_MEMORY_STATS
#define DEVTOOLS_FEATURE_USED_MEMORY_STATS

#define DEVTOOLS_FEATURE_GEDIT_VERSION 3
#define DEVTOOLS_FEATURE_SWIF_VERSION 6
