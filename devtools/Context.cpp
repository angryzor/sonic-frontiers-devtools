#include "Pch.h"
#include "Inter.h"
#include "Context.h"
#include "Desktop.h"
#include "common/Theme.h"
#include "imgui/imgui_freetype.h"
#include "imgui/ImGuizmo.h"
#include "SettingsManager.h"
#include "GOCVisualDebugDrawRenderer.h"

static ID3D11Device* device;
static ID3D11DeviceContext* deviceContext;
static ID3D11RenderTargetView* renderTargetView;
static ID3D11Texture2D* backBuffer;
static ImFont* font;

bool Context::visible = false;
bool Context::passThroughMouse = false;
bool Context::inited = false;
bool Context::alreadyRendering = false;

//SIG_SCAN
//(
//    sigSwapChainHook,
//
//    0x15575A930,
//
//    "\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x56\x57\x41\x56\x48\x81\xEC\xF0\x02", "xxxxxxxxxxxxxxxxxxx"
//);
//
//SIG_SCAN
//(
//    sigWndProcHook,
//
//    0x140BFF2B0,
//
//    "\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x55\x41\x56\x41\x57\x48\x8B\xEC\x48\x83\xEC\x70\x48\x83", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
//);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HOOK(LRESULT, __fastcall, WndProcHook, 0x140D66440, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_KEYDOWN && wParam == VK_F8) {
		Context::visible = !Context::visible;
		return true;
	}
	if (msg == WM_KEYDOWN && wParam == VK_F7 && Context::visible) {
		Context::passThroughMouse = !Context::passThroughMouse;
		return true;
	}
	if (Context::visible) {
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		ImGuiIO& io = ImGui::GetIO();

		if (io.WantCaptureMouse && msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)
			return true;
	}

	return originalWndProcHook(hWnd, msg, wParam, lParam);
}

static void createBackBuffer(IDXGISwapChain* This)
{
	if (device)
	{
		// TODO: fix scaling.
		This->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
		backBuffer->Release();
	}
}

VTABLE_HOOK(HRESULT, WINAPI, IDXGISwapChain, Present, UINT SyncInterval, UINT Flags)
{
	if (Context::visible && !Context::alreadyRendering) {
		Context::alreadyRendering = true;
		ShowCursor(true);
		if (!renderTargetView) {
			createBackBuffer(This);
		}
		Context::update();
		Context::alreadyRendering = false;
	}

	return originalIDXGISwapChainPresent(This, SyncInterval, Flags);
}

VTABLE_HOOK(HRESULT, WINAPI, IDXGISwapChain, ResizeBuffers, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	createBackBuffer(This);

	return originalIDXGISwapChainResizeBuffers(This, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

HOOK(void*, __fastcall, SwapChainHook, 0x155B35CA0, void* in_pThis, IDXGISwapChain* in_pSwapChain)
{
	INSTALL_VTABLE_HOOK(IDXGISwapChain, in_pSwapChain, Present, 8);
	INSTALL_VTABLE_HOOK(IDXGISwapChain, in_pSwapChain, ResizeBuffers, 13);

	return originalSwapChainHook(in_pThis, in_pSwapChain);
}

HOOK(void, __fastcall, MouseHookUpdate, 0x140F143C0, hh::hid::MouseWin32* a1, float a2)
{
	if (!Context::visible || Context::passThroughMouse || ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
		originalMouseHookUpdate(a1, a2);
	}
	else {
		a1->unk103 = a1->unk101;
		a1->unk105 = 0;
	}
}

void Context::install_hooks()
{
	INSTALL_HOOK(WndProcHook);
	INSTALL_HOOK(SwapChainHook);
	INSTALL_HOOK(MouseHookUpdate);
	//GOCVisualDebugDrawRenderer::InstallHooks();
}

void Context::init() {
	if (inited)
		return;

	hh::gfnd::GraphicsContext* gfx = hh::gfnd::GraphicsContext::GetInstance();
	device = static_cast<ID3D11Device*>(gfx->renderingDevice->deviceObject->objContainer->d3dDevice);

	device->GetImmediateContext(&deviceContext);

	HWND hwnd = reinterpret_cast<HWND>(hh::fw::Application::GetInstance()->frameworkEnvironment->window->hWnd);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	SettingsManager::Init();

	//static ImWchar ranges[] = { 0x1, 0xffff, 0 };
	ImFontConfig fontConfig{};
	//fontConfig.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoHinting;
	//fontConfig.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoAutoHint;
	//fontConfig.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bitmap;
	//fontConfig.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
	font = io.Fonts->AddFontFromMemoryCompressedTTF((void*)inter_compressed_data, inter_compressed_size, SettingsManager::settings.fontSize, &fontConfig);// , ranges);
	io.Fonts->Build();

	auto allocator = app::fnd::AppHeapManager::GetResidentAllocator();
	Desktop::instance = new (allocator) Desktop(allocator);

	//GOCVisualDebugDrawRenderer::instance = new (allocator) GOCVisualDebugDrawRenderer(allocator);

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, deviceContext);

	inited = true;
}

void Context::update()
{
	//if (font != nullptr && font->FontSize != SettingsManager::settings.fontSize) {
	//	ImFontConfig fontConfig{};
	//	ImGuiIO& io = ImGui::GetIO();
	//	io.Fonts->Clear();
	//	font = io.Fonts->AddFontFromMemoryCompressedTTF((void*)inter_compressed_data, inter_compressed_size, SettingsManager::settings.fontSize, &fontConfig);
	//	io.Fonts->Build();
	//}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	//GOCVisualDebugDrawRenderer::instance->Render();

	//ImGui::PushFont(firaCode);
	//ImGui::ShowDemoWindow();
	Desktop::instance->Render();

	ImGui::Render();
	//ImGui::PopFont();

	deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	//if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
	//	ImGui::UpdatePlatformWindows();
	//	ImGui::RenderPlatformWindowsDefault();
	//}
}

