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

	ResReflectionEditor(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection<void>* resource, const hh::fnd::RflClass* rflClass);
	ResReflectionEditor(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection<void>* resource);

public:
	template<typename T>
	static ResReflectionEditor* Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection<T>* resource) {
		return Create(allocator, reinterpret_cast<hh::fnd::ResReflection<void>*>(resource), rangerssdk::bootstrap::GetAddress(&T::rflClass));
	}
	static ResReflectionEditor* Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection<void>* resource, const hh::fnd::RflClass* rflClass);
	static ResReflectionEditor* Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection<void>* resource);
	
	virtual void RenderContents(); 
};
