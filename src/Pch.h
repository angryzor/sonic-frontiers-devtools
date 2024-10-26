#pragma once

#define WIN32_LEAN_AND_MEAN

// Detours
#include <Windows.h>
#include <detours.h>
#undef CreateService

// Standard library
#include <cstdint>
#include <cstdio>

#include <d3d11.h>
#include <random>

#ifdef DEVTOOLS_TARGET_SDK_wars
#include <wars-sdk.h>
#define he2sdk warssdk
#endif

#ifdef DEVTOOLS_TARGET_SDK_rangers
#include <rangers-sdk.h>
#define he2sdk rangerssdk
#endif

#ifdef DEVTOOLS_TARGET_SDK_miller
#include <miller-sdk.h>
#define he2sdk millersdk
#endif

#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include <ImGuizmo.h>
#include <ImGuiFileDialog.h>
#include <implot.h>

#include <utilities/Helpers.h>
#undef max
#undef min
