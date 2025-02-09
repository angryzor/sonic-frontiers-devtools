#include "SurfRideBugFix.h"
#include "Helpers.h"

#ifdef DEVTOOLS_TARGET_SDK_rangers
constexpr size_t surfRideCastHandlePreReloadAddr = 0x140BB5ED0;
constexpr size_t surfRideCastHandlePostReloadAddr = 0x140BB5C20;
constexpr size_t surfRideLayerHandlePreReloadAddr = 0x140BB64A0;
constexpr size_t surfRideLayerHandlePostReloadAddr = 0x140BB6210;
#endif
#ifdef DEVTOOLS_TARGET_SDK_miller
constexpr size_t surfRideCastHandlePreReloadAddr = 0x14087C700;
constexpr size_t surfRideCastHandlePostReloadAddr = 0x14087C510;
constexpr size_t surfRideLayerHandlePreReloadAddr = 0x14087CD10;
constexpr size_t surfRideLayerHandlePostReloadAddr = 0x14087CB40;
#endif
//str.copyFrom("/");
//	str.copyFrom(cast->castData->name, strlen(cast->castData->name));
//	if (cast->castData->GetType() == ucsl::resources::swif::v6::SRS_CASTNODE::Type::REFERENCE)

void AddFrag(const char* name, csl::ut::String& path) {
	if (path.size() != 0)
		path.copyFrom("/", 1, 0, path.size());
	path.copyFrom(name, strlen(name), 0, path.size());
}

void AddLayer(SurfRide::Layer* layer, csl::ut::String& path);
void AddCast(SurfRide::Cast* cast, csl::ut::String& path) {
	AddLayer(cast->layer, path);
	AddFrag(cast->castData->name, path);
}

void AddLayer(SurfRide::Layer* layer, csl::ut::String& path) {
	if (layer->referenceCast)
		AddCast(layer->referenceCast, path);
	else
		AddFrag(layer->scene->sceneData->name, path);

	AddFrag(layer->layerData->name, path);
}

const char* ReadFrag(const char* path, csl::ut::String& frag) {
	const char* splitLoc = strstr(path, "/");

	if (!splitLoc) {
		frag.copyFrom(path, strlen(path));
		return nullptr;
	}
	else {
		frag.copyFrom(path, splitLoc - path);
		return splitLoc + 1;
	}
}

SurfRide::Cast* GetCast(SurfRide::Project* project, const char* path) {
	if (path == nullptr)
		return nullptr;

	csl::ut::String frag{ hh::fnd::MemoryRouter::GetTempAllocator() };

	path = ReadFrag(path, frag);
	SurfRide::Scene* scene = project->GetScene(frag.c_str());

	if (scene == nullptr || path == nullptr)
		return nullptr;
	
	path = ReadFrag(path, frag);
	SurfRide::Layer* layer = scene->GetLayer(frag.c_str());

	if (layer == nullptr || path == nullptr)
		return nullptr;

	path = ReadFrag(path, frag);
	SurfRide::Cast* cast = layer->GetCast(frag.c_str());

	while (cast != nullptr && path != nullptr) {
		if (cast->castData->GetType() != ucsl::resources::swif::v6::SRS_CASTNODE::Type::REFERENCE)
			return nullptr;

		path = ReadFrag(path, frag);
		layer = static_cast<SurfRide::ReferenceCast*>(cast)->refLayer;

		if (strcmp(layer->name, frag.c_str()) || path == nullptr)
			return nullptr;

		path = ReadFrag(path, frag);
		cast = layer->GetCast(frag.c_str());
	}

	return cast;
}

SurfRide::Layer* GetLayer(SurfRide::Project* project, const char* path) {
	if (path == nullptr)
		return nullptr;

	csl::ut::String frag{ hh::fnd::MemoryRouter::GetTempAllocator() };

	path = ReadFrag(path, frag);
	SurfRide::Scene* scene = project->GetScene(frag.c_str());

	if (scene == nullptr || path == nullptr)
		return nullptr;
	
	path = ReadFrag(path, frag);
	SurfRide::Layer* layer = scene->GetLayer(frag.c_str());

	while (layer != nullptr && path != nullptr) {
		path = ReadFrag(path, frag);
		SurfRide::Cast* cast = layer->GetCast(frag.c_str());

		if (cast == nullptr || path == nullptr || cast->castData->GetType() != ucsl::resources::swif::v6::SRS_CASTNODE::Type::REFERENCE)
			return nullptr;

		path = ReadFrag(path, frag);
		layer = static_cast<SurfRide::ReferenceCast*>(cast)->refLayer;

		if (layer == nullptr || strcmp(layer->name, frag.c_str()))
			return nullptr;
	}

	return layer;
}

HOOK(void, __fastcall, SurfRideCastHandle_PreReload, surfRideCastHandlePreReloadAddr, hh::ui::SurfRideCastHandle* self, SurfRide::Project* project) {
	auto* cast = reinterpret_cast<SurfRide::Cast*>(reinterpret_cast<size_t>(self->surfRideObject) - 8);

	self->key.clear();

	if (cast == nullptr)
		return;

	AddCast(cast, self->key);
}

HOOK(void, __fastcall, SurfRideLayerHandle_PreReload, surfRideLayerHandlePreReloadAddr, hh::ui::SurfRideLayerHandle* self, SurfRide::Project* project) {
	auto* layer = reinterpret_cast<SurfRide::Layer*>(reinterpret_cast<size_t>(self->surfRideObject) - 8);

	self->key.clear();

	if (layer == nullptr)
		return;

	AddLayer(layer, self->key);
}

HOOK(void, __fastcall, SurfRideCastHandle_PostReload, surfRideCastHandlePostReloadAddr, hh::ui::SurfRideCastHandle* self, SurfRide::Project* project) {
	if (SurfRide::Cast* cast = GetCast(project, self->key.c_str()))
		self->surfRideObject = reinterpret_cast<void*>(reinterpret_cast<size_t>(cast) + 8);
	else
		self->surfRideObject = nullptr;
}

HOOK(void, __fastcall, SurfRideLayerHandle_PostReload, surfRideLayerHandlePostReloadAddr, hh::ui::SurfRideLayerHandle* self, SurfRide::Project* project) {
	if (SurfRide::Layer* layer = GetLayer(project, self->key.c_str()))
		self->surfRideObject = reinterpret_cast<void*>(reinterpret_cast<size_t>(layer) + 8);
	else
		self->surfRideObject = nullptr;
}

void InstallSurfRideBugFixHooks() {
	INSTALL_HOOK(SurfRideCastHandle_PreReload);
	INSTALL_HOOK(SurfRideLayerHandle_PreReload);
	INSTALL_HOOK(SurfRideCastHandle_PostReload);
	INSTALL_HOOK(SurfRideLayerHandle_PostReload);

	WRITE_MEMORY(0x140B942A6, uint8_t, 0xEB);
}
