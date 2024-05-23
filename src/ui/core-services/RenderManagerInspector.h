#pragma once
#include <ui/common/StandaloneWindow.h>

class RenderManagerInspector : public StandaloneWindow
{
public:
    RenderManagerInspector(csl::fnd::IAllocator* allocator);

    //virtual void PreRender() override;
    virtual void RenderContents() override;
};

