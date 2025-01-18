#include "DebugRenderable.h"
#include "DebugRenderer.h"

namespace devtools::debug_rendering {
	ImGuiDrawContext::ImGuiDrawContext(const Eigen::Projective3f& cameraMatrix) : cameraMatrix{ cameraMatrix } {
	}

	std::optional<ImVec2> ImGuiDrawContext::WorldCoordsToImGui(const csl::math::Vector3& vec) const {
		auto* ivp = ImGui::GetMainViewport();

		Eigen::Vector4f clipSpacePos{ cameraMatrix * vec.homogeneous() };

		// Cull
		if (clipSpacePos.x() < -clipSpacePos.w() || clipSpacePos.x() > clipSpacePos.w()
			|| clipSpacePos.y() < -clipSpacePos.w() || clipSpacePos.y() > clipSpacePos.w()
			|| clipSpacePos.z() <= 0 || clipSpacePos.z() > clipSpacePos.w())
			return std::nullopt;

		Eigen::Vector4f transformedPos{ clipSpacePos * 0.5 / clipSpacePos.w() };

		return ImVec2{ (transformedPos.x() + 0.5f) * ivp->Size.x, (0.5f - transformedPos.y()) * ivp->Size.y };
	}

	DebugRenderable::DebugRenderable() {
		DebugRenderer::instance->AddRenderable(this);
	}

	DebugRenderable::~DebugRenderable() {
		DebugRenderer::instance->RemoveRenderable(this);
	}

	void DebugRenderable::RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) {
	}

	void DebugRenderable::RenderImGuiDebugVisuals(const ImGuiDrawContext& ctx) {
	}
}
