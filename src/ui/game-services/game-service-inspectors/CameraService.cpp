#include "CameraService.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Camera.h>
#include <ui/common/editors/GraphicsContext.h>

void RenderGameServiceInspector(app::camera::CameraService& service)
{
	for (auto& bridgeUnit : service.cameraBridges) {
		if (bridgeUnit.cameraBridge.cameraFrameId != -1 && bridgeUnit.cameraComponent != nullptr && ImGui::TreeNodeEx(&bridgeUnit, ImGuiTreeNodeFlags_None, "%d", bridgeUnit.cameraBridge.cameraFrameId)) {
			ImGui::Text("Camera frame ID: %d - %s", bridgeUnit.cameraBridge.cameraFrameId, bridgeUnit.cameraBridge.cameraFrame->name.c_str());
			Editor("CameraFrame", bridgeUnit.cameraBridge.cameraFrame);
			if (ImGui::TreeNodeEx("Component", ImGuiTreeNodeFlags_None, "%s", bridgeUnit.cameraComponent->name.c_str())) {
				ImGui::Text("Viewport ID: %d", bridgeUnit.cameraComponent->viewportId);
				Editor("Viewport data", bridgeUnit.cameraComponent->viewportData);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}
