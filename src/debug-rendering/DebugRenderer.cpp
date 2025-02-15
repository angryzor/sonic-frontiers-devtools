#include "DebugRenderer.h"
#include <ui/Desktop.h>

namespace devtools::debug_rendering {
	hh::fnd::Reference<DebugRenderer> DebugRenderer::instance{};

	DebugRenderer::DebugRenderer(csl::fnd::IAllocator* allocator)
		: CompatibleObject{ allocator }
#ifdef DEVTOOLS_TARGET_SDK_wars
		, drawContext{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystem::CreateDrawContext)(allocator) }
#else
		, memCtx{ true }
		, unk2{ &memCtx }
		, unk3{ &unk2 }
		, drawContext{ RESOLVE_STATIC_VARIABLE(hh::gfnd::DrawSystem::CreateDrawContext)(allocator, &unk3) }
#endif
		, renderable{ new (allocator) Renderable(allocator, this) }
	{
		renderable->name = "DevTools Debug Overlay";
		hh::gfnd::GraphicsContext::GetInstance()->AddRenderableToViewport(renderable, 7);// 5);
	}

	void DebugRenderer::AddRenderable(DebugRenderable* renderable) {
		debugRenderables.push_back(renderable);
	}

	void DebugRenderer::RemoveRenderable(DebugRenderable* renderable) {
		debugRenderables.remove(debugRenderables.find(renderable));
	}

	void DebugRenderer::RenderImGuiVisuals()
	{
		if (auto* cameraManager = hh::game::GameManager::GetInstance()->GetService<hh::game::CameraManager>())
		if (auto* camera = cameraManager->GetTopComponent(0)) {
			Eigen::Projective3f cameraMatrix{ camera->viewportData.projMatrix * camera->viewportData.viewMatrix };
			ImGuiDrawContext ctx{ cameraMatrix };

			if (Desktop::instance->BeginOverlayWindow()) {
				for (auto* debugRenderable : debugRenderables)
					if (debugRenderable->always || enabled)
						debugRenderable->RenderImGuiDebugVisuals(ctx);

				Desktop::instance->EndOverlayWindow();
			}
		}
	}

	void DebugRenderer::Init(csl::fnd::IAllocator* allocator) {
		instance = new (allocator) DebugRenderer{ allocator };
	}

	void DebugRenderer::Deinit() {
		instance = nullptr;
	}

	DebugRenderer::Renderable::Renderable(csl::fnd::IAllocator* allocator, DebugRenderer* renderer)
#ifdef DEVTOOLS_TARGET_SDK_wars
		: hh::gfnd::Renderable{}
#else
		: hh::gfnd::Renderable{ allocator }
#endif
		, renderer{ renderer }
	{
	}

	void DebugRenderer::Renderable::Render(const hh::gfnd::RenderableParameter* renderableParameter) {
		if (auto* gameManager = hh::game::GameManager::GetInstance())
		if (auto* cameraManager = gameManager->GetService<hh::game::CameraManager>())
		if (auto* camera = cameraManager->GetTopComponent(0)) {
			renderer->drawContext->BeginScene(renderableParameter);

			hh::gfnd::DrawContext::BeginDrawInfo bdi{};
			memcpy(bdi.viewMatrix, camera->viewportData.viewMatrix.data(), sizeof(camera->viewportData.viewMatrix));
			memcpy(bdi.projMatrix, camera->viewportData.projMatrix.data(), sizeof(camera->viewportData.projMatrix));
			renderer->drawContext->BeginDraw(bdi);
			renderer->drawContext->SetCullingMode(1);

			for (auto* debugRenderable : renderer->debugRenderables)
				if (renderer->enabled || debugRenderable->always)
					debugRenderable->RenderIngameDebugVisuals(*renderer->drawContext);

			renderer->drawContext->EndDraw();
			renderer->drawContext->EndScene();
		}
	}
}
