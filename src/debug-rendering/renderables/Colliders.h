#pragma once
#include <debug-rendering/DebugRenderable.h>
#include <utilities/ColliderFilter.h>

namespace devtools::debug_rendering::renderables {
	class Colliders : public OptionalDebugRenderable {
	public:
		bool enabled{};
		bool colliderFilters[32][32];
		//ColliderFilterList colliderFilterList;

		virtual void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) override;
	};
}
