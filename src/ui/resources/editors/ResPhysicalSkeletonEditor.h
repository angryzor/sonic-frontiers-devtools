#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/common/editors/Basic.h>
#include <resources/managed-memory/ManagedCArray.h>

class ResPhysicalSkeletonEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::pba::ResPhysicalSkeleton> resource;
	ucsl::resources::pba::v1::PbaData* binaryData;
	hh::fnd::Reference<hh::pba::GOCPhysicalAnimation> gocPhys;
public:
	ResPhysicalSkeletonEditor(csl::fnd::IAllocator* allocator, hh::pba::ResPhysicalSkeleton* resource, hh::pba::GOCPhysicalAnimation* goc = nullptr);
	virtual void RenderContents();
	static ResPhysicalSkeletonEditor* Create(csl::fnd::IAllocator* allocator, hh::pba::ResPhysicalSkeleton* resource, hh::pba::GOCPhysicalAnimation* goc = nullptr);
};
