#pragma once
#include <ui/common/StandaloneWindow.h>
#include <utilities/RflDiff.h>

class ResReflectionEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::fnd::ResReflection> resource;
	RflDiffResult diffResult;
	const hh::fnd::RflClass* rflClass;
	char namebuf[500];
	bool forcedRflClass{ false };
	bool showdiff{ false };
	void* origData{};

	ResReflectionEditor(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection* resource, const hh::fnd::RflClass* rflClass);
	ResReflectionEditor(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection* resource);

public:
	virtual ~ResReflectionEditor();

	template<typename T>
	static ResReflectionEditor* Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflectionT<T>* resource) {
		return Create(allocator, reinterpret_cast<hh::fnd::ResReflection*>(resource), &RESOLVE_STATIC_VARIABLE(T::rflClass));
	}
	static ResReflectionEditor* Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection* resource, const hh::fnd::RflClass* rflClass);
	static ResReflectionEditor* Create(csl::fnd::IAllocator* allocator, hh::fnd::ResReflection* resource);

	static void RenderExportDialog();

	void MakeOriginalCopy();
	void FreeOriginalCopy();
	
	virtual void RenderContents(); 
};
