#pragma once
#include <tuple>

namespace devtools::target_config {
	namespace addresses {
		constexpr size_t appResetAddr = 0x145400300;
		constexpr size_t appShutdownAddr = 0x145401AE0;
		constexpr size_t wndProcAddr = 0x1406EC680;
		constexpr size_t displaySwapDeviceResizeBuffersAddr = 0x14082FD50;
		constexpr size_t displaySwapDevicePresentAddr = 0x1463FA880;
		constexpr size_t gocVisualDebugDrawSetup = 0x140682D50;
	}

	namespace debug_rendering {
		using default_renderables = std::tuple<
		>;
	}
}

#define DEVTOOLS_FEATURE_OLD_ARRAY
#define DEVTOOLS_FEATURE_USED_MEMORY_STATS

#define DEVTOOLS_FEATURE_GEDIT_VERSION 2
#define DEVTOOLS_FEATURE_SWIF_VERSION 5
