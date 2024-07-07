#include "GOCCamera.h"
#include <ui/common/editors/Camera.h>

void RenderComponentInspector(app_cmn::camera::GOCCamera& component)
{
	for (auto& unit : component.controllerUnits) {
		if (ImGui::TreeNodeEx(&unit, ImGuiTreeNodeFlags_None, "Controller unit %d", unit.id)) {
			if (auto* cameraFrame = static_cast<app_cmn::camera::CameraFrame*>(hh::game::GameObjectSystem::GetGameObjectByHandle(unit.cameraFrame))) {
				ImGui::Text("Camera frame: %s", cameraFrame->name.c_str());
			}

			ImGui::SeparatorText("Controller");
			ImGui::Text("Addr: %zx", unit.controller);
			ImGui::Text("Name: %s", unit.controller->name.c_str());
			if (ImGui::TreeNode("Parameters")) {
				Editor("Parameters", unit.controller->parameter);
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}
}
