#pragma once

template<typename OpModeContext>
struct MousePicking3DCameraManagerCameraProvider {
	MousePicking3DCameraManagerCameraProvider(OpModeContext& context) {}

	bool GetCameraMatrix(Eigen::Projective3f& inverseCameraMatrix) {
		if (auto* gameManager = hh::game::GameManager::GetInstance())
			if (auto* cameraSrv = gameManager->GetService<hh::game::CameraManager>())
				if (auto* camera = cameraSrv->GetTopComponent(0)) {
					inverseCameraMatrix = camera->viewportData.GetInverseViewMatrix() * camera->viewportData.projMatrix.inverse();
					return true;
				}

		return false;
	}
};
