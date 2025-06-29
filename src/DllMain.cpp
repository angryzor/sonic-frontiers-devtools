#include <ui/Context.h>
#include <ui/SettingsManager.h>
#include <utilities/SurfRideBugFix.h>

#ifdef DEVTOOLS_TARGET_SDK_rangers
FUNCTION_PTR(void, __fastcall, DeinitSCTerrain, 0x14101A3D0, hh::needle::SCTerrain*);
FUNCTION_PTR(void, __fastcall, DeinitSCTerrain1, 0x14101A410, hh::needle::SCTerrain*);
FUNCTION_PTR(void, __fastcall, DeinitSCTerrain2, 0x14101A3A0, hh::needle::SCTerrain*);

FUNCTION_PTR(void, __fastcall, DeinitSCTerrain3, 0x14101A370, hh::needle::SCTerrain*);
FUNCTION_PTR(void, __fastcall, DeinitSCTerrain4, 0x14101A3E0, hh::needle::SCTerrain*);
FUNCTION_PTR(void, __fastcall, DeinitSCTerrain5, 0x14101A490, hh::needle::SCTerrain*);

FUNCTION_PTR(void, __fastcall, DeinitTerrainComp, 0x140D0CD10, hh::gfx::TerrainComponent*);

FUNCTION_PTR(void, __fastcall, DeinitRenderingEngine, 0x140CEBB70, hh::gfx::RenderingEngineNeedle*);

template<typename T>
T ReadProtected(uintptr_t address) {
	DWORD oldProtect;
	VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), PAGE_EXECUTE_READ, &oldProtect);
	auto value = *reinterpret_cast<T*>(address);
	VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), oldProtect, &oldProtect);
	return value;
}

HOOK(void, __fastcall, HeightMapExtension_Deinitialize, 0x14C261F86, app::trr::TerrainWorldHeightMapExtension* self) {
	auto* renderManager = ReadProtected<hh::gfx::RenderManager*>(0x143D90B40);
	auto* renderingEngine = renderManager->GetNeedleResourceDevice();
	auto* supportFx = renderingEngine->GetSupportFX();
	auto* sceneContextMgr = supportFx->GetSceneContextManager(reinterpret_cast<const char*>(0));
	if (sceneContextMgr) {
		auto* scTerrain = (hh::needle::SCTerrain*)sceneContextMgr->GetSceneContext(ReadProtected<int>(0x1440C8CD8));
		if (scTerrain) {
			DeinitSCTerrain(scTerrain);
			DeinitSCTerrain1(scTerrain);
			DeinitSCTerrain2(scTerrain);
		}
		if (auto* terrainComponent = (hh::gfx::TerrainComponent*)renderingEngine->GetComponentByHash(ReadProtected<int>(0x143DC7944)))
			DeinitTerrainComp(terrainComponent);
		if (scTerrain)
		{
			DeinitSCTerrain3(scTerrain);
			DeinitSCTerrain4(scTerrain);
			DeinitSCTerrain5(scTerrain);
		}
	}
	if (renderingEngine)
		DeinitRenderingEngine(renderingEngine);
}
#endif

BOOL WINAPI DllMain(_In_ HINSTANCE hInstance, _In_ DWORD reason, _In_ LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		Context::install_hooks();
		InstallSurfRideBugFixHooks();
#ifdef DEVTOOLS_TARGET_SDK_rangers
		//INSTALL_HOOK(HeightMapExtension_Deinitialize);
#endif
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
