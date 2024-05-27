#pragma once

#include <utilities/CompatibleObject.h>

class StandaloneWindow : public CompatibleObject
{
	csl::ut::VariableString title{ GetAllocator() };
protected:
	void SetTitle(const char* title);
public:
	StandaloneWindow(csl::fnd::IAllocator* allocator);
	bool Render();
	virtual void PreRender();
	virtual void PostRender();
	virtual void RenderContents() = 0;
};

