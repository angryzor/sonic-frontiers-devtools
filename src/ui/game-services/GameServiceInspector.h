#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Reflection.h>

class GameServiceInspector : public StandaloneWindow
{
public:
    GameServiceInspector(csl::fnd::IAllocator* allocator);
    virtual void RenderContents() override;
};
