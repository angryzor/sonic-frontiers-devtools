#include <ui/Context.h>
#include <ui/SettingsManager.h>
#include <utilities/SurfRideBugFix.h>

BOOL WINAPI DllMain(_In_ HINSTANCE hInstance, _In_ DWORD reason, _In_ LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		Context::install_hooks();
		InstallSurfRideBugFixHooks();
		break;
	case DLL_PROCESS_DETACH:
		Context::deinit();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
