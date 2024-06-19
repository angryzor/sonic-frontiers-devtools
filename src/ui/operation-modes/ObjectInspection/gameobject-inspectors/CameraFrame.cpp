#include "CameraFrame.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Camera.h>

void RenderCameraBlendTree(app_cmn::camera::CameraFrame::BlendNode* node) {
	if (ImGui::TreeNode("Controller")) {
		ImGui::Text("Name: %s", node->controller->name.c_str());
		if (ImGui::TreeNode("Parameters")) {
			Editor("Parameters", node->controller->parameter);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	if (node->interpolator && ImGui::TreeNode("Interpolator")) {
		ImGui::Text("Frame id %d, name %s, address %zx", node->interpolator->cameraFrame->id, node->interpolator->cameraFrame->name.c_str(), node->interpolator->cameraFrame);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Parameters")) {
		Editor("Parameters", node->controller->parameter);
		ImGui::TreePop();
	}
	if (node->child1 && ImGui::TreeNode("Child 1")) {
		RenderCameraBlendTree(node->child1);
		ImGui::TreePop();
	}
	if (node->child2 && ImGui::TreeNode("Child 2")) {
		RenderCameraBlendTree(node->child2);
		ImGui::TreePop();
	}
}

void RenderGameObjectInspector(app_cmn::camera::CameraFrame& cameraFrame) {
    ImGui::Text("Id: %d", cameraFrame.id);
    ImGui::Text("Name: %s", cameraFrame.name.c_str());
    if (ImGui::TreeNode("Parameters 1")) {
        Editor("Parameters 1", cameraFrame.cameraParameter1);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Parameters 2")) {
        Editor("Parameters 2", cameraFrame.cameraParameter2);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Frustum 1")) {
        Editor("Frustum 1", cameraFrame.frustumParameter1);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Frustum 2")) {
        Editor("Frustum 2", cameraFrame.frustumParameter2);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Pose")) {
        Editor("Pose", cameraFrame.pose);
        ImGui::TreePop();
    }

    if (cameraFrame.blendTree && ImGui::TreeNode("Blend tree")) {
        RenderCameraBlendTree(cameraFrame.blendTree);
        ImGui::TreePop();
    }

    ImGui::SeparatorText("Controllers");
    for (auto& controllerUnit : cameraFrame.controllers) {
        ImGui::PushID(&controllerUnit);
        ImGui::Text("Addr: %zx", controllerUnit.controller);
        ImGui::Text("Name: %s", controllerUnit.controller->name.c_str());
        Editor("Camera frame", controllerUnit.controller->cameraFrame);
        ImGui::Text("Priority: %d", controllerUnit.priority);
        if (ImGui::TreeNode("Parameters")) {
            Editor("Parameters", controllerUnit.controller->parameter);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}