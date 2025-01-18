#pragma once
#include <optional>

namespace devtools::debug_rendering {
	class ImGuiDrawContext {
		const Eigen::Projective3f& cameraMatrix;
	public:
		ImGuiDrawContext(const Eigen::Projective3f& cameraMatrix);
		std::optional<ImVec2> WorldCoordsToImGui(const csl::math::Vector3& vec) const;
	};

	class DebugRenderable {
	public:
		DebugRenderable();
		virtual ~DebugRenderable();
		virtual void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx);
		virtual void RenderImGuiDebugVisuals(const ImGuiDrawContext& ctx);
	};
}
