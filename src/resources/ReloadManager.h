#pragma once
#include "utilities/FileWatch.hpp"
#include "utilities/CompatibleObject.h"

class ReloadManager : public CompatibleObject, public hh::game::GameStepListener {
    struct ReloadRequest : public CompatibleObject {
        void* ptr;
        size_t size;
        hh::fnd::Reference<hh::fnd::ManagedResource> resource;

        ReloadRequest(csl::fnd::IAllocator* allocator, void* ptr, size_t size, hh::fnd::ManagedResource* resource);
    };

    csl::ut::MoveArray<ReloadRequest*> reloadRequestsInFlight{ GetAllocator() };
    csl::ut::PointerMap<hh::fnd::ManagedResource*, void*> reloadedData{ GetAllocator() };
    CRITICAL_SECTION reloadRequestMutex{};

    filewatch::FileWatch<std::string>* fileWatcher{};

    void Reload(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource);
    void ReloadByLoad(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource);
    void ReloadSelf(hh::fnd::ManagedResource* resource);
    void ReloadObjectWorld(void* buffer, size_t fileSize, hh::game::ResObjectWorld* resource);
    void PerformReload(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource);

public:
    ReloadManager(csl::fnd::IAllocator* allocator);
    ~ReloadManager();

    static ReloadManager* instance;

    virtual void UpdateCallback(hh::game::GameManager* gameManager, const hh::game::GameStepInfo& gameStepInfo) override;
    void QueueReload(const std::wstring& path, hh::fnd::ManagedResource* resource);
    void QueueReload(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource);
    void QueueReload(hh::fnd::ManagedResource* resource);
    void ReloadSync(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource);
    void ReloadSync(hh::fnd::ManagedResource* resource);
    void WatchDirectory(const std::string& path);
};
