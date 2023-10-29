#include "Pch.h"
#include "Context.h"
#include "Desktop.h"

static ID3D11Device* device;
static ID3D11DeviceContext* deviceContext;
static ID3D11RenderTargetView* renderTargetView;
static ID3D11Texture2D* backBuffer;
static ImFont* font;

bool Context::visible = false;
bool Context::passThroughMouse = false;
bool Context::inited = false;

static Desktop* desktop{};

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
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

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
	if (Context::visible) {
		ShowCursor(true);

		if (!renderTargetView) {
			createBackBuffer(This);
		}

		Context::update();
		deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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
	if (!Context::visible || Context::passThroughMouse) {
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
}

void Context::init() {
	if (inited)
		return;

	hh::gfnd::GraphicsContext* gfx = *rangerssdk::bootstrap::GetAddress(&hh::gfnd::GraphicsContext::instance);
	device = static_cast<ID3D11Device*>(gfx->renderingDevice->deviceObject->objContainer->d3dDevice);

	device->GetImmediateContext(&deviceContext);

	void* windowData = reinterpret_cast<void**>((*rangerssdk::bootstrap::GetAddress(&hh::fw::Application::instance))->unkFromUnkParam1)[3];
	HWND hwnd = static_cast<HWND*>(windowData)[3];

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	auto allocator = app::fnd::AppHeapManager::GetResidentAllocator();
	desktop = new (allocator) Desktop(allocator);

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, deviceContext);

	inited = true;
}

void Context::update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	desktop->Render();
	//ImGui::EndFrame();

	ImGui::Render();
}

