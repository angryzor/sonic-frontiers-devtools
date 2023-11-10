#pragma once
class StandaloneWindow : public hh::fnd::ReferencedObject
{
	csl::ut::VariableString title{ GetAllocator()};
protected:
	void SetTitle(const char* title);
public:
	StandaloneWindow(csl::fnd::IAllocator* allocator);
	void Render();
	virtual void PreRender();
	virtual void RenderContents() = 0;
};

