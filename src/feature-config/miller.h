#pragma once
#include <tuple>

namespace devtools::target_config {
	namespace addresses {
		constexpr size_t appResetAddr = 0x1467BAE20;
		constexpr size_t appShutdownAddr = 0x1522F3600;
		constexpr size_t wndProcAddr = 0x140A27340;
		constexpr size_t displaySwapDeviceResizeBuffersAddr = 0x140ED9E00;
		constexpr size_t displaySwapDevicePresentAddr = 0x140ED9C30;
		constexpr size_t gocVisualDebugDrawSetup = 0x1409C4A60;
	}

	namespace memory {
		using devtools_allocator_type = hh::fnd::TlsfHeapAllocator;
	}

	namespace debug_rendering {
		using default_renderables = std::tuple<
		>;
	}
}

#define DEVTOOLS_FEATURE_PASS_ALLOCATOR
#define DEVTOOLS_FEATURE_CONFIGURABLE_COMPONENT_DATA
#define DEVTOOLS_FEATURE_TERRAIN_MANAGER
#define DEVTOOLS_FEATURE_FREE_MEMORY_STATS

#define DEVTOOLS_FEATURE_GEDIT_VERSION 3
#define DEVTOOLS_FEATURE_SWIF_VERSION 6
