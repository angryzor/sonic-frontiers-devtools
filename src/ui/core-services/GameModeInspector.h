#pragma once
#include <ui/common/StandaloneWindow.h>

class GameModeInspector : public StandaloneWindow
{
public:
    GameModeInspector(csl::fnd::IAllocator* allocator);
    virtual void RenderContents() override;
};
