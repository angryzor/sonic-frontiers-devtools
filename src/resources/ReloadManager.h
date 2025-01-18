#pragma once
#include "utilities/FileWatch.hpp"
#include "utilities/CompatibleObject.h"

class ReloadManager : public CompatibleObject, public hh::game::GameStepListener {
    struct ReloadRequest : public CompatibleObject {
        std::wstring path;
        hh::fnd::Reference<hh::fnd::ManagedResource> resource;
    public:
        ReloadRequest(csl::fnd::IAllocator* allocator, const std::wstring& path, hh::fnd::ManagedResource* resource);
    };
    csl::ut::MoveArray<ReloadRequest*> reloadRequestsInFlight{ GetAllocator() };
    CRITICAL_SECTION mutex{};
    filewatch::FileWatch<std::string>* fileWatcher{};
public:
    ReloadManager(csl::fnd::IAllocator* allocator);
    ~ReloadManager();

    static ReloadManager* instance;

    virtual void UpdateCallback(hh::game::GameManager* gameManager, const hh::game::GameStepInfo& gameStepInfo) override;
    void QueueReload(const std::wstring& path, hh::fnd::ManagedResource* resource);
    void Reload(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource);
    void ReloadByLoad(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource);
    void Reload(void* buffer, size_t fileSize, hh::game::ResObjectWorld* resource);
    void WatchDirectory(const std::string& path);
};
