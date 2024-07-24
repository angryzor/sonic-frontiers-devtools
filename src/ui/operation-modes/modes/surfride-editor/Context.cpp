#include "Context.h"

namespace ui::operation_modes::modes::surfride_editor {
#ifdef DEVTOOLS_TARGET_SDK_wars
	csl::math::Matrix44 Context::GetSceneCameraMatrix(const SurfRide::Scene* scene)
	{
		auto resolution = SurfRide::GetScreenResolution();
		auto projectionMatrix = csl::math::CreatePerspectiveProjectionMatrix(
			static_cast<float>(scene->camera.camera.cameraData.fov) * 0.000095873802,
			resolution.width / resolution.height,
			scene->camera.camera.cameraData.nearPlane,
			scene->camera.camera.cameraData.farPlane
		);
		return projectionMatrix * scene->camera.viewMatrix;
	}
#else
	csl::math::Matrix44 Context::GetSceneCameraMatrix(const SurfRide::Scene* scene)
	{
		return scene->camera.projectionMatrix * scene->camera.viewMatrix;
	}
#endif

	csl::math::Matrix44 Context::GetFocusedSceneCameraMatrix() const
	{
		return GetSceneCameraMatrix(focusedScene);
	}

	csl::math::Matrix44 Context::GetFullCastTransform(const SurfRide::Cast* cast) {
		return GetSceneCameraMatrix(cast->layer->scene) * cast->transform->transformationMatrix;
	}
}
