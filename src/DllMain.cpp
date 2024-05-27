#include <ui/Context.h>
#include <ui/SettingsManager.h>

typedef void(*BindMapsPtr)(hh::game::GameManager* a1, hh::hid::InputMapSettings* a2);

BindMapsPtr pBindMaps;

void BindMapsDetour(hh::game::GameManager* a1, hh::hid::InputMapSettings* a2) {
	pBindMaps(a1, a2);

	//a2->BindActionMapping("DmenuFastStep", 0x10000u);
	//a2->BindActionMapping("DmenuFastStep", 0x10004u);
	//a2->BindActionMapping("DmenuCursorUp", 0x10001u);
	//a2->BindActionMapping("DmenuCursorDown", 0x10003u);
	//a2->BindActionMapping("DmenuCursorLeft", 0x10000u);
	//a2->BindActionMapping("DmenuCursorRight", 0x10002u);
	//a2->BindActionMapping("DmenuCursorUp", 0x20052u);
	//a2->BindActionMapping("DmenuCursorDown", 0x20051u);
	//a2->BindActionMapping("DmenuCursorLeft", 0x20050u);
	//a2->BindActionMapping("DmenuCursorRight", 0x2004Fu);
	//a2->BindActionMapping("DmenuDecide", 0x10004u);
	//a2->BindActionMapping("DmenuDecide", 0x20028u);
	//a2->BindActionMapping("DmenuDecide", 0x20058u);
	//a2->BindActionMapping("DmenuDecide", 0x10005u);
	//a2->BindActionMapping("DmenuCancel", 0x20029u);
	//a2->BindActionMapping("DmenuCancel", 0x40003u);

	// Debug camera, gamepad bindings
	//a2->BindActionMapping("HHFreeCameraSwitchArcballCamera", 0x10007u);

	a2->BindActionMapping("HHFreeCameraSpeedChange", 0x10005u);
	a2->BindActionMapping("HHFreeCameraReset", 0x10004u);
	a2->BindActionMapping("HHFreeCameraRoll", 0x1000cu);
	a2->BindActionMapping("HHFreeCameraDistance", 0x1000du);
	a2->BindActionMapping("HHFreeCameraFovy", 0x1000fu);
	a2->BindActionMapping("HHFreeCameraUpDown", 0x1000du);
	a2->BindActionMapping("HHFreeCameraSwitchViewport", 0x10006u);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x10001u, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x10003u, -1.0);
	a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x10000u, -1.0);
	a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x10002u, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x10009u, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x10008u, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x1000bu, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x1000au, 1.0);

	//a2->BindActionMapping("HHFreeCameraArcballCameraRotate", 0x2001fu);
	//a2->BindActionMapping("HHFreeCameraArcballCameraTransXY", 0x20020u);
	//a2->BindActionMapping("HHFreeCameraArcballCameraTransZ", 0x20021u);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20052u, 1.0);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20051u, -1.0);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x20050u, -1.0);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x2004fu, 1.0);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002du, -1.0);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002eu, 1.0);

	// Debug camera, keyboard bindings
	//a2->BindActionMapping("HHFreeCameraSwitchArcballCamera", 0x2001eu);

	// Movement: WASD & arrows
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x20052u, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x20051u, -1.0);
	a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x20050u, -1.0);
	a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x2004fu, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x2001au, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x20016u, -1.0);
	a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x20004u, -1.0);
	a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x20007u, 1.0);

	// Look: keypad arrows
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x20060u, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2005au, -1.0);
	a2->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x2005cu, -1.0);
	a2->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x2005eu, 1.0);

	// Up/down: hold ctrl + move & pgup/dn
	a2->BindActionMapping("HHFreeCameraUpDown", 0x200e0u);
	a2->BindActionMapping("HHFreeCameraUpDown", 0x200e4u);
	a2->BindActionMapping("HHFreeCameraUpDown", 0x2004bu);
	a2->BindActionMapping("HHFreeCameraUpDown", 0x2004eu);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x2004bu, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveVertical", 0x2004eu, -1.0);

	// Speed
	a2->BindActionMapping("HHFreeCameraSpeedChange", 0x200e1u);
	a2->BindActionMapping("HHFreeCameraSpeedChange", 0x200e5u);

	// Reset
	a2->BindActionMapping("HHFreeCameraReset", 0x2003au);

	// Roll: q/e
	a2->BindActionMapping("HHFreeCameraRoll", 0x20014u);
	a2->BindActionMapping("HHFreeCameraRoll", 0x20008u);
	a2->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x20014u, -1.0);
	a2->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x20008u, 1.0);

	// Zoom: +/-
	a2->BindActionMapping("HHFreeCameraDistance", 0x2002eu);
	a2->BindActionMapping("HHFreeCameraDistance", 0x2002du);
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2002eu, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2002du, -1.0);
	a2->BindActionMapping("HHFreeCameraDistance", 0x20057u);
	a2->BindActionMapping("HHFreeCameraDistance", 0x20056u);
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x20057u, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x20056u, -1.0);

	// Fovy: home/end
	a2->BindActionMapping("HHFreeCameraFovy", 0x2004au);
	a2->BindActionMapping("HHFreeCameraFovy", 0x2004du);
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2004au, 1.0);
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2004du, -1.0);

	// Change viewport: KP *
	a2->BindActionMapping("HHFreeCameraSwitchViewport", 0x20055u);

	//a2->BindActionMapping("HHFreeCameraArcballCameraRotate", 0x2001fu);
	//a2->BindActionMapping("HHFreeCameraArcballCameraTransXY", 0x20020u);
	//a2->BindActionMapping("HHFreeCameraArcballCameraTransZ", 0x20021u);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20052u, 1.0);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20051u, -1.0);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x20050u, -1.0);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x2004fu, 1.0);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002du, -1.0);
	//a2->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002eu, 1.0);

	// Mouse (to be used in conjuction with keyboard)
	a2->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x40001u, -SettingsManager::settings.debugCameraMouseSensitivityY);
	a2->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x40000u, SettingsManager::settings.debugCameraMouseSensitivityX);

	// Move up/down
	//a2->BindActionMapping("HHFreeCameraUpDown", 0x30005u);
	//a2->BindActionMapping("HHFreeCameraUpDown", 0x30006u);
	//a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x30005u, -1.0);
	//a2->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x30006u, 1.0);
}



BOOL WINAPI DllMain(_In_ HINSTANCE hInstance, _In_ DWORD reason, _In_ LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
			//pGameModeStageRebuildLevel = reinterpret_cast<GameModeStageRebuildLevelPtr>(reinterpret_cast<size_t>(baseAddress) + 0x01F5B90);
		//pGameModeBootInit = reinterpret_cast<GameModeBootInitPtr>(0x14734FB80);
		//pBindMaps = reinterpret_cast<BindMapsPtr>(0x14025ED30);

		Context::install_hooks();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		//DetourAttach(&(PVOID&)pGameModeStageRebuildLevel, &GameModeStageRebuildLevelDetour);
		//DetourAttach(&(PVOID&)pGameModeBootInit, &GameModeBootInitDetour);
		//DetourAttach(&(PVOID&)pBindMaps, &BindMapsDetour);
		DetourTransactionCommit();
	break;
	case DLL_PROCESS_DETACH:
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		//DetourDetach(&(PVOID&)pGameModeStageRebuildLevel, &GameModeStageRebuildLevelDetour);
		//DetourDetach(&(PVOID&)pGameModeBootInit, &GameModeBootInitDetour);
		//DetourDetach(&(PVOID&)pBindMaps, &BindMapsDetour);
		DetourTransactionCommit();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
