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
#include <rangers-sdk.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/implot.h"

#include "Helpers.h"
#undef max
