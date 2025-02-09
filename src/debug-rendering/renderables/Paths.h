#pragma once
#include <debug-rendering/DebugRenderable.h>

namespace devtools::debug_rendering::renderables {
	class Paths : public OptionalDebugRenderable {
	public:
		bool enabled{};

		virtual void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) override;
		virtual void RenderImGuiDebugVisuals(const ImGuiDrawContext& ctx) override;
	};
}
