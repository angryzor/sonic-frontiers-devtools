#pragma once
#include <ui/common/StandaloneWindow.h>

class RenderingEngineInspector : public StandaloneWindow
{
public:
    RenderingEngineInspector(csl::fnd::IAllocator* allocator);

    //virtual void PreRender() override;
    virtual void RenderContents() override;
};

