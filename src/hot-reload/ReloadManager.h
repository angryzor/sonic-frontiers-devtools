#pragma once
#include "utilities/FileWatch.hpp"

class ReloadManager : public hh::fnd::ReferencedObject, public hh::game::GameStepListener {
    struct ReloadRequest : public hh::fnd::BaseObject {
        std::wstring path;
        hh::fnd::Reference<hh::fnd::ManagedResource> resource;
    public:
        ReloadRequest(csl::fnd::IAllocator* allocator, const std::wstring& path, hh::fnd::ManagedResource* resource);
    };
    volatile PVOID reloadRequestInFlight{};
    filewatch::FileWatch<std::string>* fileWatcher{};
public:
    ReloadManager(csl::fnd::IAllocator* allocator);
    ~ReloadManager();

    static ReloadManager* instance;

    virtual void UpdateCallback(hh::game::GameManager* gameManager, const hh::fnd::SUpdateInfo& updateInfo) override;
    void QueueReload(const std::wstring& path, hh::fnd::ManagedResource* resource);
    void Reload(void* buffer, size_t fileSize, hh::fnd::ManagedResource* resource);
    void Reload(void* buffer, size_t fileSize, hh::game::ResObjectWorld* resource);
    void WatchDirectory(const std::string& path);
};
