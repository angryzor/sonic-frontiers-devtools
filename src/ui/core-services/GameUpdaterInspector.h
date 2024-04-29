#pragma once
#include <ui/common/StandaloneWindow.h>

class GameUpdaterInspector : public StandaloneWindow {
public:
    GameUpdaterInspector(csl::fnd::IAllocator* allocator);

    virtual void PreRender() override;
    virtual void RenderContents() override;
};
