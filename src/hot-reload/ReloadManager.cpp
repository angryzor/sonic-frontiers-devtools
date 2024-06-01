#include "ReloadManager.h"
#include <ui/Desktop.h>
#include <ui/operation-modes/LevelEditor/LevelEditor.h>
#include <utilities/ResourceTypes.h>
#include <locale>
#include <codecvt>
#include <filesystem>

using namespace hh::fnd;
using namespace hh::game;

ReloadManager* ReloadManager::instance{};

ReloadManager::ReloadManager(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator } {
    GameManager::GetInstance()->RegisterGameStepListener(*this);
}

ReloadManager::~ReloadManager() {
	if (fileWatcher != nullptr) {
		fileWatcher->~FileWatch();
		GetAllocator()->Free(fileWatcher);
	}

    if (auto* gameManager = GameManager::GetInstance())
        gameManager->UnregisterGameStepListener(*this);

    if (auto prevPtr = static_cast<ReloadRequest*>(InterlockedExchangePointer(&reloadRequestInFlight, nullptr)))
        delete prevPtr;
}

void ReloadManager::QueueReload(const std::wstring& path, ManagedResource* resource)
{
	if (auto prevPtr = static_cast<ReloadRequest*>(InterlockedExchangePointer(&reloadRequestInFlight, new (GetAllocator()) ReloadRequest{ GetAllocator(), path, resource })))
		delete prevPtr;
}

void ReloadManager::UpdateCallback(GameManager* gameManager, const hh::game::GameStepInfo& gameStepInfo)
{
    if (auto* request = static_cast<ReloadRequest*>(InterlockedExchangePointer(&reloadRequestInFlight, nullptr))) {
		HANDLE file = CreateFileW(request->path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE) {
			delete request;
			return;
		}

		DWORD fileSize = GetFileSize(file, NULL);
		void* buffer = request->resource->GetResourceAllocator()->Alloc(fileSize, 64);
		bool result = ReadFile(file, buffer, fileSize, NULL, NULL);
		CloseHandle(file);

		if (!result) {
			request->resource->GetResourceAllocator()->Free(buffer);
			delete request;
			return;
		}

		auto* gameManager = hh::game::GameManager::GetInstance();
		//if (gameManager) {
		//	gameManager->PreResourceReloadCallback(request->resource);
		//	if (auto* trrMgr = gameManager->GetService<app::trr::TerrainManager>())
		//		trrMgr->reloaderListener->PreResourceReloadCallback(request->resource);
		//}

		if (&request->resource->GetClass() == hh::game::ResObjectWorld::GetTypeInfo())
			Reload(buffer, fileSize, static_cast<ResObjectWorld*>(&*request->resource));
		//else if (&request->resource->GetClass() == heur::resources::ResTerrainModel::GetTypeInfo())
		//	ReloadByLoad(buffer, fileSize, request->resource);
		else
			Reload(buffer, fileSize, request->resource);

		//if (gameManager) {
		//	gameManager->PostResourceReloadCallback(request->resource);
		//	if (auto* trrMgr = gameManager->GetService<app::trr::TerrainManager>())
		//		trrMgr->reloaderListener->PostResourceReloadCallback(request->resource);
		//}

		//if (&request->resource->GetClass() == heur::resources::ResTerrainModel::GetTypeInfo()
		//	|| &request->resource->GetClass() == app::gfx::ResPointcloudModel::GetTypeInfo()
		//	|| &request->resource->GetClass() == hh::gfx::ResMaterial::GetTypeInfo()) {
		//	if (auto* resMan = gameManager->GetService<app::game::GameModeResourceManager>()) {
		//		for (auto& module : resMan->modules) {
		//			if (module->GetNameHash() == 0x74DA1FC3) {
		//				module->UnkFunc7();
		//				module->Load();
		//			}
		//		}
		//	}
		//}

		delete request;
    }
}

void ReloadManager::Reload(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource)
{
	hh::fnd::ResourceManagerResolver resolver{};
	resource->Reload(buffer, fileSize);
	resource->Resolve(resolver);
	resource->GetResourceAllocator()->Free(buffer);
}

void ReloadManager::ReloadByLoad(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource)
{
	hh::fnd::ResourceManagerResolver resolver{};
	resource->Unload();
	resource->Load(buffer, fileSize);
	resource->Resolve(resolver);
	resource->GetResourceAllocator()->Free(buffer);
}

void ReloadManager::Reload(void* buffer, size_t fileSize, hh::game::ResObjectWorld* resource)
{
	bool success{ false };

	if (auto* gameManager = hh::game::GameManager::GetInstance()) {
		if (auto* objWorld = gameManager->GetService<hh::game::ObjectWorld>()) {
			for (auto* chunk : objWorld->GetWorldChunks()) {
				if (auto* layer = chunk->GetLayerByName(resource->GetName())) {
					hh::ut::BinaryFile bfile{ buffer };

					if (bfile.IsValid()) {
						auto* allocator = layer->GetAllocator();
						bool wasEnabled = layer->IsEnable();

						//if (auto* levelEditor = dynamic_cast<LevelEditor*>(&*Desktop::instance->operationMode))
						//	levelEditor->Deselect();

						{
							Reference<hh::game::ObjectWorldChunkLayer> l{ layer };
							chunk->RemoveLayer(layer);
							chunk->ShutdownPendingObjects();
						}

						resource->GetResourceAllocator()->Free(resource->originalBinaryData);
						resource->originalBinaryData = buffer;
						resource->unpackedBinaryData = bfile.GetDataAddress(-1);
						resource->size = fileSize;
						resource->Load(resource->unpackedBinaryData, resource->size);

						chunk->AddLayer(hh::game::ObjectWorldChunkLayer::Create(allocator, resource));
						chunk->SetLayerEnabled(resource->GetName(), wasEnabled);

						//if (auto* levelEditor = dynamic_cast<LevelEditor*>(&*Desktop::instance->operationMode))
						//	levelEditor->ReloadObjectWorldData();

						success = true;
						break;
					}
				}
			}
		}
	}

	if (!success)
		resource->GetResourceAllocator()->Free(buffer);
}

void ReloadManager::WatchDirectory(const std::string& path) {
	if (fileWatcher != nullptr) {
		fileWatcher->~FileWatch();
		GetAllocator()->Free(fileWatcher);
	}

	fileWatcher = new (GetAllocator()) filewatch::FileWatch<std::string>(path, [this,path](const std::string& subpath, const filewatch::Event changeType) {
		switch (changeType) {
		case filewatch::Event::added:
		case filewatch::Event::modified:
			{
				std::filesystem::path fspath{ path };
				fspath /= subpath;

				if (!std::filesystem::exists(fspath))
					return;

				std::wstring wfilename = fspath.filename();
				std::wstring wext = fspath.extension();

				std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
				std::string filename = converter.to_bytes(wfilename);
				size_t dotloc = filename.find('.');

				if (dotloc == std::string::npos)
					return;

				std::string fullExt = filename.substr(dotloc + 1);
				auto* resourceType = GetTypeInfoByExtension(fullExt.c_str());

				if (resourceType == nullptr)
					return;

				auto* resource = ResourceManager::GetInstance()->GetResource(filename.c_str(), resourceType);

				if (resource == nullptr)
					return;

				QueueReload(fspath, resource);
			}
			break;
		}
	});
}


ReloadManager::ReloadRequest::ReloadRequest(csl::fnd::IAllocator* allocator, const std::wstring& path, hh::fnd::ManagedResource* resource)
	: CompatibleObject{ allocator }, path{ path }, resource{ resource }
{
}
