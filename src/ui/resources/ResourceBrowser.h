#pragma once
#include <filesystem>
#include <ui/common/StandaloneWindow.h>
#include "utilities/FileWatch.hpp"

class ResourceBrowser : public StandaloneWindow
{
    csl::ut::MoveArray<hh::fnd::Reference<hh::fnd::Packfile>> currentPath{ GetAllocator() };
    hh::fnd::Reference<hh::fnd::DynamicResourceContainer> selectedResourceContainer{};
    hh::fnd::Reference<hh::fnd::ManagedResource> selectedResource{};

    static filewatch::FileWatch<std::string>* fileWatcher;
public:
    ResourceBrowser(csl::fnd::IAllocator* allocator);

    virtual void PreRender() override;
    virtual void RenderContents() override;
    void RenderMainArea();
    void RenderResource(hh::fnd::ManagedResource* resource);
    void RenderContainerContents(const hh::fnd::ResourceContainer * container);
    static void RenderPreview(const hh::fnd::ManagedResource* resource, float size);
    static void WatchDirectory(const std::string& path);
    static void ReloadResource(const wchar_t* filePath, hh::fnd::ManagedResource* resource);
    static void ExportResource(const wchar_t* filePath, hh::fnd::ManagedResource* resource);
    static void ShowLoadResourceDialog(hh::fnd::ManagedResource* resource);
    static void ShowExportResourceDialog(hh::fnd::ManagedResource* resource);
    static const char* GetExtensionByTypeInfo(const hh::fnd::ResourceTypeInfo* typeInfo);
    static const hh::fnd::ResourceTypeInfo* GetTypeInfoByExtension(const char* extension);
    static void RenderLoadDialog();
    static void RenderExportDialog();
    static void RenderDialogs();
    //void RenderResources();
};

