#pragma once
#include <utilities/math/MathUtils.h>

template<typename OpModeContext>
struct MousePicking3DMousePickingBehaviorTraitsImpl {
	using ObjectType = typename MousePicking3DBehaviorTraits<OpModeContext>::ObjectType;
	using LocationType = typename MousePicking3DBehaviorTraits<OpModeContext>::LocationType;

	MousePicking3DBehaviorTraits<OpModeContext> traits;
	MousePicking3DCameraProvider<OpModeContext> cameraProvider;

	MousePicking3DMousePickingBehaviorTraitsImpl(OpModeContext& context) : traits{ context }, cameraProvider{ context } {}

	template<typename F>
	void WithCameraMatrix(F f) {
		Eigen::Matrix4f inverseCameraMatrix;

		if (!cameraProvider.GetCameraMatrix(inverseCameraMatrix))
			return;

		f(inverseCameraMatrix);
	}

	void GetAllClickResults(const ImVec2& position, csl::ut::MoveArray<ObjectType>& results) {
		return WithCameraMatrix([&](const Eigen::Matrix4f& inverseCameraMatrix) {
			traits.GetAllRaycastResults(ScreenPosToWorldRay(position, inverseCameraMatrix), results);
		});
	}

	void GetBestClickResult(const ImVec2& position, csl::ut::MoveArray<ObjectType>& results, typename LocationType& location, bool& pickedLocation) {
		return WithCameraMatrix([&](const Eigen::Matrix4f& inverseCameraMatrix) {
			traits.GetBestRaycastResult(ScreenPosToWorldRay(position, inverseCameraMatrix), results, location, pickedLocation);
		});
	}

	void GetDragResults(const ImVec2& start, const ImVec2 end, csl::ut::MoveArray<ObjectType>& results) {
		return WithCameraMatrix([&](const Eigen::Matrix4f& inverseCameraMatrix) {
			traits.GetFrustumResults(ScreenRectToFrustum(start, end, inverseCameraMatrix), results);
		});
	}

	const char* GetObjectName(ObjectType object) {
		return traits.GetObjectName(object);
	}
};
