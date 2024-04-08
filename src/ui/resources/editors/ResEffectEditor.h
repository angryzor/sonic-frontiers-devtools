#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/common/ReflectionEditor.h>
#include <reflection/serialization/ReflectionSerializer.h>

namespace heur::resources {
	class ResEffect;
}

class ResEffectEditor : public StandaloneWindow {
	hh::fnd::Reference<heur::resources::ResEffect> resource;
public:
	ResEffectEditor(csl::fnd::IAllocator* allocator, heur::resources::ResEffect* resource);
	virtual void RenderContents();
};
