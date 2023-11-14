#include "Pch.h"
#include <rangers-sdk.h>
#include "Context.h"


static void* baseAddress;
//static hh::fnd::Reference<hh::fnd::ResourceLoader> resourceLoader;
//
//typedef uint64_t(*GameModeStageRebuildLevelPtr)(uint64_t unk1, app::game::GameMode* gameMode, uint64_t unk3);
//
//GameModeStageRebuildLevelPtr pGameModeStageRebuildLevel;
//
//static bool created{ false };

typedef uint64_t(*GameModeBootInitPtr)(app::game::GameMode* gameModeBoot);

GameModeBootInitPtr pGameModeBootInit;

//class DevEventInfo : public hh::game::GameService {
//public:
//	static hh::game::GameServiceClass* GetClass() { return reinterpret_cast<hh::game::GameServiceClass*>(0x143DAB358); }
//};

uint64_t GameModeBootInitDetour(app::game::GameMode* gameModeBoot) {
	auto res = pGameModeBootInit(gameModeBoot);

	////auto allocators = hh::fnd::GetAllocatorSystem();
	//auto allocator = app::fnd::AppHeapManager::GetResidentAllocator();
	//resourceLoader = new (allocator) hh::fnd::ResourceLoader(allocator);
	//resourceLoader->LoadPackfile("mods/debugtools/segafont_art.pac", true);

	//auto service = gameModeBoot->gameManager->CreateService<DevEventInfo>(allocator);
	//gameModeBoot->gameManager->RegisterService(service);
	Context::init();

	return res;
}

WNDPROC pWndProc;

LRESULT WndProcDetour(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	return pWndProc(hWnd, msg, wParam, lParam);
}

//class ComponentInspector {
//public:
//	static ComponentInspector* (***componentInspectors)(csl::fnd::IAllocator*);
//	void (**vfuncs)(ComponentInspector* self);
//	char unk[1952];
//	hh::game::GOComponent* component;
//};
//
//namespace heur::gocs {
//	class GOCVisualModel : public hh::game::GOComponent {
//	public:
//		static hh::game::GOComponentClass* GetClass();
//	};
//}

//uint64_t GameModeStageRebuildLevelDetour(uint64_t unk1, app::game::GameMode* gameMode, uint64_t unk3) {
//	auto res = pGameModeStageRebuildLevel(unk1, gameMode, unk3);
//
//	//auto menuSelect = reinterpret_cast<hh::game::GameObjectClass*>(reinterpret_cast<size_t>(baseAddress) + 0x3CCA0A0)->Instantiate<app::game::MenuSelect>(gameMode->GetAllocator());
//	//gameMode->gameManager->RegisterNamedObject(menuSelect, nullptr, false, nullptr, nullptr);
//
//
//	//unsigned int col = 0xFFFFFFFF;
//	//menuSelect->menu->item_1->SetUnk4(&col);
//	//menuSelect->menu->item_1->MaybeOpen(true);
//	//auto tinyDevMenu = reinterpret_cast<hh::game::GameObjectClass*>(reinterpret_cast<size_t>(baseAddress) + 0x3CCA110)->Instantiate<hh::game::GameObject>(gameMode->GetAllocator());
//	//gameMode->gameManager->RegisterNamedObject(tinyDevMenu, nullptr, false, nullptr, nullptr);
//
//	//*reinterpret_cast<csl::fnd::IAllocator**>(0x1440EB2C0) = gameMode->GetAllocator();
//
//	//auto inspector = reinterpret_cast<ComponentInspector* (*)(csl::fnd::IAllocator*)>(0x140CF6A60)(gameMode->GetAllocator());
//	//inspector->component = gameMode->gameManager->GetGameObject("Sonic")->GetComponentUnsafe<heur::gocs::GOCVisualModel>();
//	//inspector->vfuncs[59](inspector);
//
//	return res;
//
//	//auto tinyDevMenu = reinterpret_cast<hh::game::GameObjectClass*>(reinterpret_cast<size_t>(baseAddress) + 0x3CCA040)->Instantiate<hh::game::GameObject>(gameMode->GetAllocator());
//	//gameMode->gameManager->RegisterNamedObject(tinyDevMenu, nullptr, false, nullptr, nullptr);
//	//return res;
//}

typedef void(*BindMapsPtr)(hh::game::GameManager* a1, hh::hid::InputMapSettings* a2);

BindMapsPtr pBindMaps;

void BindMapsDetour(hh::game::GameManager* a1, hh::hid::InputMapSettings* a2) {
	pBindMaps(a1, a2);

	//a2->BindActionMapping("DmenuFastStep", 0x10000u, -1);
	//a2->BindActionMapping("DmenuFastStep", 0x10004u, -1);
	//a2->BindActionMapping("DmenuCursorUp", 0x10001u, -1);
	//a2->BindActionMapping("DmenuCursorDown", 0x10003u, -1);
	//a2->BindActionMapping("DmenuCursorLeft", 0x10000u, -1);
	//a2->BindActionMapping("DmenuCursorRight", 0x10002u, -1);
	//a2->BindActionMapping("DmenuCursorUp", 0x20052u, -1);
	//a2->BindActionMapping("DmenuCursorDown", 0x20051u, -1);
	//a2->BindActionMapping("DmenuCursorLeft", 0x20050u, -1);
	//a2->BindActionMapping("DmenuCursorRight", 0x2004Fu, -1);
	//a2->BindActionMapping("DmenuDecide", 0x10004u, -1);
	//a2->BindActionMapping("DmenuDecide", 0x20028u, -1);
	//a2->BindActionMapping("DmenuDecide", 0x20058u, -1);
	//a2->BindActionMapping("DmenuDecide", 0x10005u, -1);
	//a2->BindActionMapping("DmenuCancel", 0x20029u, -1);
	//a2->BindActionMapping("DmenuCancel", 0x40003u, -1);

	// Debug camera, gamepad bindings
	//a2->BindActionMapping("HHFreeCameraSwitchArcballCamera", 0x10007u, -1);

	a2->BindActionMapping("HHFreeCameraSpeedChange", 0x10005u, -1);
	a2->BindActionMapping("HHFreeCameraReset", 0x10004u, -1);
	a2->BindActionMapping("HHFreeCameraRoll", 0x1000cu, -1);
	a2->BindActionMapping("HHFreeCameraDistance", 0x1000du, -1);
	a2->BindActionMapping("HHFreeCameraFovy", 0x1000fu, -1);
	a2->BindActionMapping("HHFreeCameraUpDown", 0x1000du, -1);
	a2->BindActionMapping("HHFreeCameraSwitchViewport", 0x10006u, -1);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x10001u, 1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x10003u, -1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x10000u, -1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x10002u, 1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x10009u, 1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x10008u, 1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x1000bu, 1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x1000au, 1.0, -1);

	//a2->BindActionMapping("HHFreeCameraArcballCameraRotate", 0x2001fu, -1);
	//a2->BindActionMapping("HHFreeCameraArcballCameraTransXY", 0x20020u, -1);
	//a2->BindActionMapping("HHFreeCameraArcballCameraTransZ", 0x20021u, -1);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20052u, 1.0, -1);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20051u, -1.0, -1);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x20050u, -1.0, -1);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x2004fu, 1.0, -1);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002du, -1.0, -1);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002eu, 1.0, -1);

	// Debug camera, keyboard bindings
	//a2->BindActionMapping("HHFreeCameraSwitchArcballCamera", 0x2001eu, -1);

	a2->BindActionMapping("HHFreeCameraSpeedChange", 0x20020u, -1);
	a2->BindActionMapping("HHFreeCameraReset", 0x2001fu, -1);
	a2->BindActionMapping("HHFreeCameraRoll", 0x20021u, -1);
	a2->BindActionMapping("HHFreeCameraDistance", 0x20022u, -1);
	a2->BindActionMapping("HHFreeCameraFovy", 0x20023u, -1);
	a2->BindActionMapping("HHFreeCameraUpDown", 0x20022u, -1);
	a2->BindActionMapping("HHFreeCameraSwitchViewport", 0x20024u, -1);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x20052u, 1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x20051u, -1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x20050u, -1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x2004fu, 1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x20060u, 1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2005au, -1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x2005cu, -1.0, -1);
	a2->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x2005eu, 1.0, -1);

	//a2->BindActionMapping("HHFreeCameraArcballCameraRotate", 0x2001fu, -1);
	//a2->BindActionMapping("HHFreeCameraArcballCameraTransXY", 0x20020u, -1);
	//a2->BindActionMapping("HHFreeCameraArcballCameraTransZ", 0x20021u, -1);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20052u, 1.0, -1);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20051u, -1.0, -1);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x20050u, -1.0, -1);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x2004fu, 1.0, -1);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002du, -1.0, -1);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002eu, 1.0, -1);
}
BOOL WINAPI DllMain(_In_ HINSTANCE hInstance, _In_ DWORD reason, _In_ LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		{
			baseAddress = GetModuleHandle(nullptr);

			rangerssdk::SetBaseAddress(baseAddress);

			//pGameModeStageRebuildLevel = reinterpret_cast<GameModeStageRebuildLevelPtr>(reinterpret_cast<size_t>(baseAddress) + 0x01F5B90);
			pGameModeBootInit = reinterpret_cast<GameModeBootInitPtr>(reinterpret_cast<size_t>(baseAddress) + 0x76832E0);
			pBindMaps = reinterpret_cast<BindMapsPtr>(reinterpret_cast<size_t>(baseAddress) + 0x25E0F0);
		}

		Context::install_hooks();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		//DetourAttach(&(PVOID&)pGameModeStageRebuildLevel, &GameModeStageRebuildLevelDetour);
		DetourAttach(&(PVOID&)pGameModeBootInit, &GameModeBootInitDetour);
		DetourAttach(&(PVOID&)pBindMaps, &BindMapsDetour);
		DetourTransactionCommit();
	break;
	case DLL_PROCESS_DETACH:
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		//DetourDetach(&(PVOID&)pGameModeStageRebuildLevel, &GameModeStageRebuildLevelDetour);
		DetourDetach(&(PVOID&)pGameModeBootInit, &GameModeBootInitDetour);
		DetourDetach(&(PVOID&)pBindMaps, &BindMapsDetour);
		DetourTransactionCommit();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
