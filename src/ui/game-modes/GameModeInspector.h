#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/common/ReflectionEditor.h>

class GameModeInspector : public StandaloneWindow
{
public:
    GameModeInspector(csl::fnd::IAllocator* allocator);
    virtual void RenderContents() override;
};
