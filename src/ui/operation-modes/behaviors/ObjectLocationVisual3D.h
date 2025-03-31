#pragma once
#include <debug-rendering/DebugRenderable.h>
#include <debug-rendering/DebugRenderer.h>
#include <ui/common/overlays/Tag.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include "Selection.h"
#include "ForwardDeclarations.h"

class ObjectLocationVisual3DBehaviorBase : public OperationModeBehavior, public devtools::debug_rendering::DebugRenderable {
protected:
	hh::fnd::Reference<hh::gfnd::GraphicsGeometry> targetBox{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystem::CreateGraphicsGeometry)(nullptr, GetAllocator()) };
	hh::fnd::Reference<hh::gfnd::GraphicsGeometry> selectedTargetBox{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystem::CreateGraphicsGeometry)(nullptr, GetAllocator()) };

	inline static float debugBoxScale{ 0.03f };
	inline static bool scaleChanged{ false };

	inline static unsigned int renderLimit{ 500 };
	inline static float renderDistance{ 500.0f };
	inline static bool renderPrimaryTags{ false };
	inline static bool renderSecondaryTags{ false };
public:
	ObjectLocationVisual3DBehaviorBase(csl::fnd::IAllocator* allocator, OperationModeBase& operationMode) : OperationModeBehavior{ allocator, operationMode }, DebugRenderable{} {
		UpdateDebugBoxGeometry();
	}

	static constexpr unsigned int id = 4;
	virtual unsigned int GetId() override { return id; }

	static void ApplySettings(float scale, unsigned int limit, float distance, bool renderPrimary, bool renderSecondary) {
		debugBoxScale = scale;
		scaleChanged = true;
		renderLimit = limit;
		renderDistance = distance;
		renderPrimaryTags = renderPrimary;
		renderSecondaryTags = renderSecondary;
	}

	void Render() {
		if (scaleChanged) {
			UpdateDebugBoxGeometry();
			scaleChanged = false;
		}
	}

	void UpdateDebugBoxGeometry() {
		hh::fnd::Geometry box{ GetAllocator() };
		box.CreateBox({ 0, 0, 0 }, { debugBoxScale, debugBoxScale, debugBoxScale }, csl::math::Quaternion::Identity());
		targetBox->Initialize(devtools::debug_rendering::DebugRenderer::instance->drawContext, box);
		targetBox->SetColor({ 255, 0, 255, 255 });
		selectedTargetBox->Initialize(devtools::debug_rendering::DebugRenderer::instance->drawContext, box);
		selectedTargetBox->SetColor({ 255, 255, 0, 255 });
	}
};

template<typename OpModeContext>
class ObjectLocationVisual3DBehavior : public ObjectLocationVisual3DBehaviorBase {
public:
	using Traits = ObjectLocationVisual3DBehaviorTraits<OpModeContext>;
	using ObjectType = typename SelectionBehaviorTraits<OpModeContext>::ObjectType;

private:
	Traits traits;

public:
	ObjectLocationVisual3DBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode) : ObjectLocationVisual3DBehaviorBase{ allocator, operationMode }, traits{ operationMode.GetContext() } {}

	virtual void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) override {
		auto* selectionBehavior = operationMode.GetBehavior<SelectionBehavior<OpModeContext>>();
		unsigned int idx{};

		if (auto* gameManager = hh::game::GameManager::GetInstance())
		if (auto* cameraMgr = gameManager->GetService<hh::game::CameraManager>())
		if (auto* camera = cameraMgr->GetTopComponent(0)) {
			Eigen::Vector3f cameraPos = camera->viewportData.GetInverseViewMatrix().translation();

			traits.ForInvisibleObjects([&](ObjectType object) {
				if (idx++ >= renderLimit)
					return;

				auto transform = traits.GetWorldTransform(object);
				Eigen::Vector3f position = transform.translation();

				if ((position - cameraPos).norm() > renderDistance)
					return;

				(selectionBehavior->GetSelection().find(object) != -1 ? selectedTargetBox : targetBox)->Render(&ctx, transform);
			});
		}
	}

	virtual void RenderImGuiDebugVisuals(const devtools::debug_rendering::ImGuiDrawContext& ctx) override {
		unsigned int idx{};

		if (!renderPrimaryTags)
			return;
		
		if (auto* gameManager = hh::game::GameManager::GetInstance())
		if (auto* cameraMgr = gameManager->GetService<hh::game::CameraManager>())
		if (auto* camera = cameraMgr->GetTopComponent(0)) {
			Eigen::Vector3f cameraPos = camera->viewportData.GetInverseViewMatrix().translation();

			traits.ForAllObjects([&](ObjectType object) {
				if (idx++ >= renderLimit)
					return;
			
				auto transform = traits.GetWorldTransform(object);
				Eigen::Vector3f position = transform.translation();

				if ((position - cameraPos).norm() > renderDistance)
					return;

				auto maybePos = ctx.WorldCoordsToImGui(position);

				if (!maybePos.has_value())
					return;

				auto* selectionBehavior = operationMode.GetBehavior<SelectionBehavior<OpModeContext>>();

				if (OverlayTag(traits.GetPrimaryTag(object), selectionBehavior ? selectionBehavior->GetSelection().find(object) != -1 : false, maybePos.value(), ImVec4(1.0f, 0.0f, 1.0f, 1.0f)) && selectionBehavior)
					selectionBehavior->Select(object);

				if (Traits::dragDropCategory && ImGui::BeginDragDropSource()) {
					ImGui::SetDragDropPayload(Traits::dragDropCategory, &object, sizeof(object));
					ImGui::EndDragDropSource();
				}
			});
		}
	}
};
