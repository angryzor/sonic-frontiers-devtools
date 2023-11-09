#pragma once
#include "StandaloneWindow.h"

class ResReflectionEditorWindow : public StandaloneWindow
{
	Reference<hh::fnd::ResReflection> resource;
	const RflClass* rflClass

public:
	ResReflectionEditorWindow(csl::fnd::IAllocator* allocator);
	virtual void Render() override;
};

