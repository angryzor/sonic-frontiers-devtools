#include "DebugCamera.h"
#include "Common.h"

namespace devtools::api::debug_camera {
	struct DebugCameraSettings {
		Vector3 origin{};
		Vector3 position{};
		float yaw{};
		float pitch{};
		float roll{};
		float zoom{};
		float nearClip{};
		float farClip{};
		float fov{};
		float speed{};
	};

	struct DebugCameraState {
		bool enabled{};
		std::optional<DebugCameraSettings> settings{};
	};

	void RegisterRoutes(APIContext& ctx) {
		ctx.app->get("/debug-camera", [&ctx](auto* res, auto* req) {
			ctx.apiCall(res, []() {
				auto* debugCameraMgr = hh::game::DebugCameraManager::GetInstance();

				if (!debugCameraMgr->isActive)
					return DebugCameraState{ .enabled = false };

#ifdef DEVTOOLS_TARGET_SDK_wars
				auto& camCtrl = *((hh::game::FreeCameraController*)&*debugCameraMgr->GetCamera()->controller);
#else
				auto& camCtrl = *((hh::game::DefaultFreeCameraController*)&*debugCameraMgr->GetCamera()->controller)->padController;
#endif

				return DebugCameraState{
					.enabled = true,
					.settings = DebugCameraSettings{
						.origin = {.x = camCtrl.unk3.camera.origin.x(), .y = camCtrl.unk3.camera.origin.y(), .z = camCtrl.unk3.camera.origin.z() },
						.position = {.x = camCtrl.unk3.camera.position.x(), .y = camCtrl.unk3.camera.position.y(), .z = camCtrl.unk3.camera.position.z() },
						.yaw = camCtrl.unk3.camera.yaw,
						.pitch = camCtrl.unk3.camera.pitch,
						.roll = camCtrl.unk3.camera.roll,
						.zoom = camCtrl.unk3.camera.zoom,
						.nearClip = camCtrl.unk3.viewport.nearClip,
						.farClip = camCtrl.unk3.viewport.farClip,
						.fov = camCtrl.unk3.viewport.fov,
						.speed = camCtrl.currentSpeed,
					},
				};
			});
		});
		ctx.app->put("/debug-camera", [&ctx](auto* res, auto* req) {
			ctx.apiCallWithBody<DebugCameraState>(res, [](const DebugCameraState& body) {
				auto* debugCameraMgr = hh::game::DebugCameraManager::GetInstance();

				if (debugCameraMgr->isActive != body.enabled) {
					if (body.enabled)
						debugCameraMgr->ActivateDebugCamera({ 1, 0, 1, 0 });
					else
						debugCameraMgr->DeactivateDebugCamera();
				}

				if (debugCameraMgr->isActive && body.settings.has_value()) {
#ifdef DEVTOOLS_TARGET_SDK_wars
					auto& camCtrl = *((hh::game::FreeCameraController*)&*debugCameraMgr->GetCamera()->controller);
#else
					auto& camCtrl = *((hh::game::DefaultFreeCameraController*)&*debugCameraMgr->GetCamera()->controller)->padController;
#endif

					auto& settings = body.settings.value();

					camCtrl.unk3.camera.origin = { settings.origin.x, settings.origin.y, settings.origin.z };
					camCtrl.unk3.camera.position = { settings.position.x, settings.position.y, settings.position.z };
					camCtrl.unk3.camera.yaw = settings.yaw;
					camCtrl.unk3.camera.pitch = settings.pitch;
					camCtrl.unk3.camera.roll = settings.roll;
					camCtrl.unk3.camera.zoom = settings.zoom;
					camCtrl.unk3.viewport.nearClip = settings.nearClip;
					camCtrl.unk3.viewport.farClip = settings.farClip;
					camCtrl.unk3.viewport.fov = settings.fov;
					camCtrl.currentSpeed = settings.speed;
				}

				return Empty{};
			});
		});
	}
}
