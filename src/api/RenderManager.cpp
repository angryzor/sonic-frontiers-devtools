#ifndef DEVTOOLS_TARGET_SDK_wars
#include "RenderManager.h"
#include "Common.h"

namespace devtools::api::render_manager {
	struct CameraSettings {
		Vector3 position{};
		Vector3 rotation{};
		float nearClip{};
		float farClip{};
		float fov{};
	};

	struct ViewportSettings {
		std::optional<CameraSettings> camera{};
		unsigned int x{};
		unsigned int y{};
		unsigned int resX{};
		unsigned int resY{};
		unsigned int prevRenderResX{};
		unsigned int prevRenderResY{};
		unsigned int renderResX{};
		unsigned int renderResY{};
	};

	std::optional<CameraSettings> buildCamera(hh::needle::FxCamera* camera){
		if(camera){
			float nearClip = 0.0f;
			float farClip = 0.0f;
			camera->GetClipPlanes(&nearClip, &farClip);
			return CameraSettings{
				.position = {.x = camera->GetEyePos().x(), .y = camera->GetEyePos().y(), .z = camera->GetEyePos().z()},
				.rotation = {.x = camera->GetViewMatrix().rotation()(0), .y = camera->GetViewMatrix().rotation()(1), .z = camera->GetViewMatrix().rotation()(2)},
				.nearClip = nearClip,
				.farClip = farClip,
				.fov = camera->GetFieldOfView()
			};
		}
		else
			return std::nullopt;
	}

	ViewportSettings buildViewport(hh::needle::SupportFX::FxViewport* viewport) {
		return ViewportSettings{
			.camera = buildCamera(viewport->camera),
			.x = viewport->x,
			.y = viewport->y,
			.resX = viewport->resX,
			.resY = viewport->resY,
			.prevRenderResX = viewport->prevRenderResX,
			.prevRenderResY = viewport->prevRenderResY,
			.renderResX = viewport->renderResX,
			.renderResY = viewport->renderResY
		};
	}

	hh::gfx::RenderManager::Impl::LocalFxCamera* getCamera(hh::gfx::RenderManager* renderMgr, size_t cameraId){
		if(cameraId > 3){
			throw ErrorResponse{
				.status = "404 Not Found",
				.message = "cameraId: invalid id",
			};
		}

		return &renderMgr->implementation->fxCameras[cameraId];
	}

	hh::needle::SupportFX::FxViewport* getViewport(hh::gfx::RenderManager* renderMgr, size_t viewportId){
		if(viewportId > 3){
			throw ErrorResponse{
				.status = "404 Not Found",
				.message = "viewportId: invalid id",
			};
		}

		return &renderMgr->implementation->viewports[viewportId];
	}

	void RegisterRoutes(APIContext& ctx) {
		ctx.app->get("/render-manager/cameras/:cameraId", [&ctx](auto* res, auto* req) {
			ctx.apiCall(res, [cameraId = ctx.getParameter<size_t>(req, "cameraId")]() {
				auto* renderMgr = static_cast<hh::gfx::RenderManager*>(hh::gfx::RenderManager::GetInstance());
				auto* camera = getCamera(renderMgr, cameraId);
				return buildCamera(camera);
			});
		});
		ctx.app->get("/render-manager/viewports/:viewportId", [&ctx](auto* res, auto* req) {
			ctx.apiCall(res, [viewportId = ctx.getParameter<size_t>(req, "viewportId")]() {
				auto* renderMgr = static_cast<hh::gfx::RenderManager*>(hh::gfx::RenderManager::GetInstance());
				auto* viewport = getViewport(renderMgr, viewportId);
				return buildViewport(viewport);
			});
		});
		ctx.app->put("/render-manager/viewports/:viewportId", [&ctx](auto* res, auto* req) {
			ctx.apiCallWithBody<ViewportSettings>(res, [viewportId = ctx.getParameter<size_t>(req, "viewportId")](const ViewportSettings& body) {
				auto* renderMgr = static_cast<hh::gfx::RenderManager*>(hh::gfx::RenderManager::GetInstance());
				auto* viewport = getViewport(renderMgr, viewportId);
				if(viewport){
					auto& settings = body;
					viewport->x = settings.x;
					viewport->y = settings.y;
					viewport->resX = settings.resX;
					viewport->resY = settings.resY;
					viewport->prevRenderResX = settings.prevRenderResX;
					viewport->prevRenderResY = settings.prevRenderResY;
					viewport->renderResX = settings.renderResX;
					viewport->renderResY = settings.renderResY;
				}

				return Empty{};
			});
		});
	}
}
#endif
