#pragma once
#include "StandaloneWindow.h"

class ResourceBrowser : public StandaloneWindow
{
    csl::ut::MoveArray<hh::fnd::Reference<hh::fnd::Packfile>> currentPath{ GetAllocator() };
    hh::fnd::Reference<hh::fnd::DynamicResourceContainer> selectedResourceContainer{};
    hh::fnd::Reference<hh::fnd::ManagedResource> selectedResource{};
public:
    ResourceBrowser(csl::fnd::IAllocator* allocator);

    virtual void Render() override;
    void RenderMainArea();
    void RenderResource(hh::fnd::ManagedResource* resource);
    void RenderContainerContents(const hh::fnd::ResourceContainer * container);
    //void RenderResources();
};

