#include "fonts/Inter.h"
#include "common/Theme.h"
#include "Context.h"
#include "Desktop.h"
#include "SettingsManager.h"
#include <debug-rendering/GOCVisualDebugDrawRenderer.h>
//#include <hot-reload/ReloadManager.h>

static ID3D11Device* device;
static ID3D11DeviceContext* deviceContext;
static ID3D11RenderTargetView* renderTargetView;
static ID3D11Texture2D* backBuffer;
static ImFont* font;

bool Context::visible = false;
bool Context::passThroughMouse = false;
bool Context::inited = false;
bool Context::alreadyRendering = false;

HOOK(uint64_t, __fastcall, ApplicationStart, 0x145400300, hh::game::GameApplication* application) {
	auto res = originalApplicationStart(application);

	auto allocator = hh::fnd::MemoryRouter::GetModuleAllocator();
	auto* resourceLoader = new (allocator) hh::fnd::ResourceLoader();
	resourceLoader->LoadPackfile("mods/debugtools/segafont_art.pac", true);

	//auto service = gameModeBoot->gameManager->CreateService<DevEventInfo>(allocator);
	//gameModeBoot->gameManager->RegisterService(service);
	Context::init();

	return res;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HOOK(LRESULT, __fastcall, WndProcHook, 0x1406EC680, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

		if (io.WantCaptureKeyboard && msg >= WM_KEYFIRST && msg <= WM_KEYLAST)
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

HOOK(void*, __fastcall, SwapChainHook, 0x14082EE90, void* in_pThis, IDXGISwapChain* in_pSwapChain)
{
	INSTALL_VTABLE_HOOK(IDXGISwapChain, in_pSwapChain, Present, 8);
	INSTALL_VTABLE_HOOK(IDXGISwapChain, in_pSwapChain, ResizeBuffers, 13);

	return originalSwapChainHook(in_pThis, in_pSwapChain);
}

//HOOK(void, __fastcall, MouseHookUpdate, 0x140F16F00, hh::hid::MouseWin32* a1, float a2)
//{
//	if (!Context::visible || Context::passThroughMouse || ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
//		originalMouseHookUpdate(a1, a2);
//	}
//	else {
//		a1->unk103 = a1->unk101;
//		a1->unk105 = 0;
//	}
//}

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

HOOK(hh::hid::InputMapSettings*, __fastcall, BindMaps, 0x1401862B0, csl::fnd::IAllocator* allocator) {
	auto* inputSettings = originalBindMaps(allocator);

	//inputSettings->BindActionMapping("DmenuFastStep", 0x10000u);
	//inputSettings->BindActionMapping("DmenuFastStep", 0x10004u);
	//inputSettings->BindActionMapping("DmenuCursorUp", 0x10001u);
	//inputSettings->BindActionMapping("DmenuCursorDown", 0x10003u);
	//inputSettings->BindActionMapping("DmenuCursorLeft", 0x10000u);
	//inputSettings->BindActionMapping("DmenuCursorRight", 0x10002u);
	//inputSettings->BindActionMapping("DmenuCursorUp", 0x20052u);
	//inputSettings->BindActionMapping("DmenuCursorDown", 0x20051u);
	//inputSettings->BindActionMapping("DmenuCursorLeft", 0x20050u);
	//inputSettings->BindActionMapping("DmenuCursorRight", 0x2004Fu);
	//inputSettings->BindActionMapping("DmenuDecide", 0x10004u);
	//inputSettings->BindActionMapping("DmenuDecide", 0x20028u);
	//inputSettings->BindActionMapping("DmenuDecide", 0x20058u);
	//inputSettings->BindActionMapping("DmenuDecide", 0x10005u);
	//inputSettings->BindActionMapping("DmenuCancel", 0x20029u);
	//inputSettings->BindActionMapping("DmenuCancel", 0x40003u);

	// Debug camera, gamepad bindings
	//inputSettings->BindActionMapping("HHFreeCameraSwitchArcballCamera", 0x10007u);

	inputSettings->BindActionMapping("HHFreeCameraSpeedChange", 0x10005u);
	inputSettings->BindActionMapping("HHFreeCameraReset", 0x10004u);
	inputSettings->BindActionMapping("HHFreeCameraRoll", 0x1000cu);
	inputSettings->BindActionMapping("HHFreeCameraDistance", 0x1000du);
	inputSettings->BindActionMapping("HHFreeCameraFovy", 0x1000fu);
	inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x1000du);
	inputSettings->BindActionMapping("HHFreeCameraSwitchViewport", 0x10006u);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x10001u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x10003u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x10000u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x10002u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x10009u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x10008u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x1000bu, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x1000au, 1.0);

	//inputSettings->BindActionMapping("HHFreeCameraArcballCameraRotate", 0x2001fu);
	//inputSettings->BindActionMapping("HHFreeCameraArcballCameraTransXY", 0x20020u);
	//inputSettings->BindActionMapping("HHFreeCameraArcballCameraTransZ", 0x20021u);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20052u, 1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20051u, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x20050u, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x2004fu, 1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002du, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002eu, 1.0);

	// Debug camera, keyboard bindings
	//inputSettings->BindActionMapping("HHFreeCameraSwitchArcballCamera", 0x2001eu);

	// Movement: WASD & arrows
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x20052u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x20051u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x20050u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x2004fu, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x2001au, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x20016u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x20004u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x20007u, 1.0);

	// Look: keypad arrows
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x20060u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2005au, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x2005cu, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x2005eu, 1.0);

	// Up/down: hold ctrl + move & pgup/dn
	inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x200e0u);
	inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x200e4u);
	inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x2004bu);
	inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x2004eu);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x2004bu, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x2004eu, -1.0);

	// Speed
	inputSettings->BindActionMapping("HHFreeCameraSpeedChange", 0x200e1u);
	inputSettings->BindActionMapping("HHFreeCameraSpeedChange", 0x200e5u);

	// Reset
	inputSettings->BindActionMapping("HHFreeCameraReset", 0x2003au);

	// Roll: q/e
	inputSettings->BindActionMapping("HHFreeCameraRoll", 0x20014u);
	inputSettings->BindActionMapping("HHFreeCameraRoll", 0x20008u);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x20014u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x20008u, 1.0);

	// Zoom: +/-
	inputSettings->BindActionMapping("HHFreeCameraDistance", 0x2002eu);
	inputSettings->BindActionMapping("HHFreeCameraDistance", 0x2002du);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2002eu, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2002du, -1.0);
	inputSettings->BindActionMapping("HHFreeCameraDistance", 0x20057u);
	inputSettings->BindActionMapping("HHFreeCameraDistance", 0x20056u);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x20057u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x20056u, -1.0);

	// Fovy: home/end
	inputSettings->BindActionMapping("HHFreeCameraFovy", 0x2004au);
	inputSettings->BindActionMapping("HHFreeCameraFovy", 0x2004du);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2004au, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2004du, -1.0);

	// Change viewport: KP *
	inputSettings->BindActionMapping("HHFreeCameraSwitchViewport", 0x20055u);

	//inputSettings->BindActionMapping("HHFreeCameraArcballCameraRotate", 0x2001fu);
	//inputSettings->BindActionMapping("HHFreeCameraArcballCameraTransXY", 0x20020u);
	//inputSettings->BindActionMapping("HHFreeCameraArcballCameraTransZ", 0x20021u);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20052u, 1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20051u, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x20050u, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x2004fu, 1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002du, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002eu, 1.0);

	// Mouse (to be used in conjuction with keyboard)
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x40001u, -SettingsManager::settings.debugCameraMouseSensitivityY);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x40000u, SettingsManager::settings.debugCameraMouseSensitivityX);

	// Move up/down
	//inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x30005u);
	//inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x30006u);
	//inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x30005u, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x30006u, 1.0);

	return inputSettings;
}

void Context::install_hooks()
{
	INSTALL_HOOK(ApplicationStart);
	INSTALL_HOOK(WndProcHook);
	INSTALL_HOOK(SwapChainHook);
	INSTALL_HOOK(BindMaps);
	//INSTALL_HOOK(MouseHookUpdate);
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
	auto* allocator = moduleAllocator;
	//static hh::fnd::ThreadSafeTlsfHeapAllocator devtoolsAllocator{ "devtools" };
	//devtoolsAllocator.Setup(moduleAllocator, { 100 * 1024 * 1024, true });
	//auto* allocator = &devtoolsAllocator;

	//ReloadManager::instance = new (allocator) ReloadManager(allocator);
	RESOLVE_STATIC_VARIABLE(hh::game::DebugCameraManager::instance) = hh::game::DebugCameraManager::Create();
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

