#pragma once
#include "../common/ReflectionEditor.h"
#include "../serialization/ReflectionSerializer.h"
#include "../imgui/ImGuiFileDialog.h"
#include "../StandaloneWindow.h"

class ResReflectionEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::fnd::ResReflection<void>> resource;
	const hh::fnd::RflClass* rflClass;
	char namebuf[500];
	bool forcedRflClass{ false };
public:
	ResReflectionEditor(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection<void>* resource, const hh::fnd::RflClass* rflClass);
	ResReflectionEditor(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection<void>* resource);
	virtual void RenderContents(); 
};
