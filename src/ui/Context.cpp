#include "fonts/Inter.h"
#include "common/Theme.h"
#include "Context.h"
#include "input/Input.h"
#include "Desktop.h"
#include "SettingsManager.h"
#include <debug-rendering/GOCVisualDebugDrawRenderer.h>
#include <hot-reload/ReloadManager.h>

static ID3D11Device* device;
static ID3D11DeviceContext* deviceContext;
static ID3D11RenderTargetView* renderTargetView;
static ID3D11Texture2D* backBuffer;
static ImFont* font;

bool Context::visible = false;
bool Context::passThroughMouse = false;
bool Context::inited = false;
bool Context::alreadyRendering = false;

using namespace hh::game;
using namespace hh::needle;

#ifdef DEVTOOLS_TARGET_SDK_wars
constexpr size_t appResetAddr = 0x145400300;
constexpr size_t wndProcAddr = 0x1406EC680;
constexpr size_t displaySwapDeviceConstructorAddr = 0x14082EE90;
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
constexpr size_t appResetAddr = 0x1501A41F0;
constexpr size_t wndProcAddr = 0x140D68F80;
constexpr size_t displaySwapDeviceConstructorAddr = 0x155D23F80;
#endif

HOOK(uint64_t, __fastcall, GameApplication_Reset, appResetAddr, hh::game::GameApplication* self) {
	auto res = originalGameApplication_Reset(self);
	Context::init();
	return res;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HOOK(LRESULT, __fastcall, WndProcHook, wndProcAddr, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

		if (ShouldCaptureMouseInputs() && msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)
			return true;

		if (ShouldCaptureKeyboardInputs() && msg >= WM_KEYFIRST && msg <= WM_KEYLAST)
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

HOOK(void*, __fastcall, SwapChainHook, displaySwapDeviceConstructorAddr, void* in_pThis, IDXGISwapChain* in_pSwapChain)
{
	INSTALL_VTABLE_HOOK(IDXGISwapChain, in_pSwapChain, Present, 8);
	INSTALL_VTABLE_HOOK(IDXGISwapChain, in_pSwapChain, ResizeBuffers, 13);

	return originalSwapChainHook(in_pThis, in_pSwapChain);
}

//HOOK(bool, __fastcall, CreateRenderingDeviceDX11, 0x1410EC330, hh::needle::RenderingDevice** renderingDevice, hh::needle::RenderingDeviceContext** renderingDeviceContext, void* deviceCreationSetting, void** displaySwapDevice, unsigned int creationFlags)
//{
//	return originalCreateRenderingDeviceDX11(renderingDevice, renderingDeviceContext, deviceCreationSetting, displaySwapDevice, D3D11_CREATE_DEVICE_DEBUG);
//}

//HOOK(bool, __fastcall, GOCCamera_PushController, 0x14D39B880, app_cmn::camera::GOCCamera* self, hh::fnd::Handle<hh::fnd::Messenger>& cameraFrame, unsigned int controllerId, unsigned int unkParam1, app_cmn::camera::CameraInterpolator* interpolator)
//{
//	if (self->owner->objectClass == app::player::Sonic::GetClass() || self->owner->objectClass == app::player::Amy::GetClass() || self->owner->objectClass == app::player::Knuckles::GetClass() || self->owner->objectClass == app::player::Tails::GetClass()) {
//		auto* player = static_cast<app::player::Player*>(self->owner);
//		hh::fnd::Handle<hh::fnd::Messenger> cameraFrame2 = hh::game::GameManager::GetInstance()->GetService<app::camera::CameraService>()->GetCameraFrame(player->setupInfo.playerId * 2);
//		return originalGOCCamera_PushController(self, cameraFrame2, controllerId, unkParam1, interpolator);
//	}
//	else
//		return originalGOCCamera_PushController(self, cameraFrame, controllerId, unkParam1, interpolator);
//}

void Context::install_hooks()
{
	INSTALL_HOOK(GameApplication_Reset);
	INSTALL_HOOK(WndProcHook);
	INSTALL_HOOK(SwapChainHook);
	InstallInputHooks();
	//INSTALL_HOOK(RenderingEngineRangers_SetupMainRenderUnit);
	//INSTALL_HOOK(GOCCamera_PushController);
	//INSTALL_HOOK(CreateRenderingDeviceDX11);
	GOCVisualDebugDrawRenderer::InstallHooks();
}

void Context::init() {
	if (inited)
		return;

	device = static_cast<hh::gfx::RenderManager*>(hh::gfx::RenderManager::GetInstance())->GetNeedleResourceDevice()->GetNativeDevice();

	device->GetImmediateContext(&deviceContext);

	HWND hwnd = reinterpret_cast<HWND>(hh::fw::Application::GetInstance()->frameworkEnvironment->window->hWnd);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
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

	auto* moduleAllocator = hh::fnd::MemoryRouter::GetModuleAllocator();
	//auto* allocator = moduleAllocator;
	static hh::fnd::ThreadSafeTlsfHeapAllocator devtoolsAllocator{ "devtools" };
	devtoolsAllocator.Setup(moduleAllocator, { 100 * 1024 * 1024, true });
	auto* allocator = &devtoolsAllocator;

	ReloadManager::instance = new (allocator) ReloadManager(allocator);
#ifdef DEVTOOLS_TARGET_SDK_wars
	RESOLVE_STATIC_VARIABLE(hh::game::DebugCameraManager::instance) = hh::game::DebugCameraManager::Create();
#endif
	GOCVisualDebugDrawRenderer::instance = new (allocator) GOCVisualDebugDrawRenderer(allocator);
	Desktop::instance = new (allocator) Desktop{ allocator };

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

	//ImGui::PushFont(firaCode);
	//ImGui::ShowDemoWindow();
	//ImPlot::ShowDemoWindow();
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

