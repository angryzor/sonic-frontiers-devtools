#include "Pch.h"
#include "GOCVisualDebugDrawRenderer.h"

using namespace hh::game;
using namespace hh::gfx;

class GOCMyVisualDebugDraw : public GOCVisualDebugDraw {
public:
	GOCVisualDebugDraw::DebugGeometryDefinition def;
	csl::ut::Color<uint8_t> color{ 0, 0, 0, 0 };
	bool hasGeometry;

	GOCMyVisualDebugDraw(csl::fnd::IAllocator* allocator);
	virtual void OnGOCEvent(GOCEvent event, GameObject& ownerGameObject, void* data);

	static void InstallHooks();

	GOCOMPONENT_CLASS_DECLARATION(GOCMyVisualDebugDraw)
};

GOCMyVisualDebugDraw* GOCMyVisualDebugDraw::Create(csl::fnd::IAllocator* allocator) {
	return new (std::align_val_t(alignof(GOCMyVisualDebugDraw)), allocator) GOCMyVisualDebugDraw(allocator);
}

GOCMyVisualDebugDraw::GOCMyVisualDebugDraw(csl::fnd::IAllocator* allocator) : GOCVisualDebugDraw{ allocator }, def{ allocator } {
}

void GOCMyVisualDebugDraw::OnGOCEvent(GOCEvent event, GameObject& ownerGameObject, void* data) {
	if (event == GOCEvent::ACTIVATE)
		GOCVisualDebugDrawRenderer::instance->AddGOC(this);
	if (event == GOCEvent::DEACTIVATE)
		GOCVisualDebugDrawRenderer::instance->RemoveGOC(this);
	
	GOCVisualDebugDraw::OnGOCEvent(event, ownerGameObject, data);
}

void GOCMyVisualDebugDraw::InstallHooks() {
	WRITE_MEMORY(0x143CE89E8, void*, &GOCMyVisualDebugDraw::Create);
}

HOOK(bool, __fastcall, VisualDebugDrawSetup, 0x14F973750, GOCMyVisualDebugDraw* gocVisual, const GOCVisualDebugDraw::SetupInfo& setupInfo) {
	bool ret = originalVisualDebugDrawSetup(gocVisual, setupInfo);

	if (setupInfo.geomDef != nullptr) {
		for (auto& vertex : setupInfo.geomDef->vertices)
			gocVisual->def.vertices.push_back(vertex);

		for (auto& triangle : setupInfo.geomDef->triangles)
			gocVisual->def.triangles.push_back(triangle);

		gocVisual->hasGeometry = true;
	}

	gocVisual->color = setupInfo.color;

	return ret;
}

hh::fnd::Reference<GOCVisualDebugDrawRenderer> GOCVisualDebugDrawRenderer::instance = nullptr;

void GOCVisualDebugDrawRenderer::InstallHooks() {
	INSTALL_HOOK(VisualDebugDrawSetup);
	GOCMyVisualDebugDraw::InstallHooks();
}

GOCVisualDebugDrawRenderer::GOCVisualDebugDrawRenderer(csl::fnd::IAllocator* allocator) : ReferencedObject{ allocator, true }
{
}

void GOCVisualDebugDrawRenderer::AddGOC(GOCMyVisualDebugDraw* goc) {
	gocs.push_back(goc);
}

void GOCVisualDebugDrawRenderer::RemoveGOC(GOCMyVisualDebugDraw* goc) {
	gocs.remove(gocs.find(goc));
}

void GOCVisualDebugDrawRenderer::Render() {
	auto& io = ImGui::GetIO();

	auto* gameManager = hh::game::GameManager::GetInstance();
	if (!gameManager)
		return;

	auto* camera = gameManager->GetService<hh::game::CameraManager>()->GetTopComponent(0);
	if (!camera)
		return;

	Eigen::Matrix4f cameraMat{ camera->viewportData.projMatrix * camera->viewportData.viewMatrix };

	for (auto* goc : gocs) {
		if ((goc->frame2->fullTransform.position - camera->viewportData.unk6).norm() > 300)
			continue;

		uint32_t color = (goc->color.a << IM_COL32_A_SHIFT) | (goc->color.b << IM_COL32_B_SHIFT) | (goc->color.g << IM_COL32_G_SHIFT) | (goc->color.r << IM_COL32_R_SHIFT);
		uint32_t fillColor = (16 << IM_COL32_A_SHIFT) | (goc->color.b << IM_COL32_B_SHIFT) | (goc->color.g << IM_COL32_G_SHIFT) | (goc->color.r << IM_COL32_R_SHIFT);

		Eigen::Transform<float, 3, Eigen::Affine> absoluteTransform{};
		absoluteTransform.fromPositionOrientationScale(goc->frame2->fullTransform.position, goc->frame2->fullTransform.rotation, goc->frame2->fullTransform.scale);

		Eigen::Matrix4f totalMat{ cameraMat * absoluteTransform.matrix() };

		for (auto& triangle : goc->def.triangles) {
			Eigen::Vector4f hv1{ totalMat * goc->def.vertices[triangle.v1].homogeneous() };
			Eigen::Vector4f hv2{ totalMat * goc->def.vertices[triangle.v2].homogeneous() };
			Eigen::Vector4f hv3{ totalMat * goc->def.vertices[triangle.v3].homogeneous() };

			// Very QnD cull on Z axis instead of proper clipping
			if (hv1.z() <= 0 || hv1.z() > hv1.w() || hv2.z() <= 0 || hv2.z() > hv2.w() || hv3.z() <= 0 || hv3.z() > hv3.w())
				continue;

			Eigen::Vector4f v1{ hv1 * 0.5 / hv1.w() };
			Eigen::Vector4f v2{ hv2 * 0.5 / hv2.w() };
			Eigen::Vector4f v3{ hv3 * 0.5 / hv3.w() };

			ImGui::GetBackgroundDrawList()->AddTriangleFilled(
				ImVec2((v1.x() + 0.5f) * io.DisplaySize.x, (0.5f - v1.y()) * io.DisplaySize.y),
				ImVec2((v2.x() + 0.5f) * io.DisplaySize.x, (0.5f - v2.y()) * io.DisplaySize.y),
				ImVec2((v3.x() + 0.5f) * io.DisplaySize.x, (0.5f - v3.y()) * io.DisplaySize.y),
				fillColor
			);

			ImGui::GetBackgroundDrawList()->AddTriangle(
				ImVec2((v1.x() + 0.5f) * io.DisplaySize.x, (0.5f - v1.y()) * io.DisplaySize.y),
				ImVec2((v2.x() + 0.5f) * io.DisplaySize.x, (0.5f - v2.y()) * io.DisplaySize.y),
				ImVec2((v3.x() + 0.5f) * io.DisplaySize.x, (0.5f - v3.y()) * io.DisplaySize.y),
				color
			);
		}
	}
}
