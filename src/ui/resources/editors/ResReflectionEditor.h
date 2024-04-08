#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/common/ReflectionEditor.h>
#include <reflection/serialization/ReflectionSerializer.h>

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
		return Create(allocator, reinterpret_cast<hh::fnd::ResReflection<void>*>(resource), rangerssdk::GetAddress(&T::rflClass));
	}
	static ResReflectionEditor* Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection<void>* resource, const hh::fnd::RflClass* rflClass);
	static ResReflectionEditor* Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection<void>* resource);
	
	virtual void RenderContents(); 
};
