#include "Camera.h"
#include <ui/common/editors/Basic.h>

using namespace app_cmn::camera;

bool Editor(const char* label, CameraParameter::Target& target) {
    bool edited{};
    ImGui::PushID(label);
    
	edited |= Editor("Position", target.position);
	edited |= Editor("Offset", target.offset);
	edited |= Editor("Unk3", target.unk3);

    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, CameraParameter::Offset& offset) {
    bool edited{};
    ImGui::PushID(label);
    
	edited |= Editor("Offset", reinterpret_cast<csl::math::Vector3&>(offset));

    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, CameraParameter::Orientation& orientation) {
    bool edited{};
    ImGui::PushID(label);
    
	edited |= Editor("Unk7", orientation.unk7);
	edited |= Editor("Unk8", orientation.unk8);
	edited |= Editor("Unk9", orientation.unk9);
	edited |= Editor("Unk8b", orientation.unk8b);

    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, CameraParameter& parameter) {
    bool edited{};
    ImGui::PushID(label);

	edited |= Editor("Target", parameter.target);
	edited |= Editor("Offset", parameter.offset);
	edited |= Editor("Orientation", parameter.orientation);

    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, FrustumParameter& parameter) {
    bool edited{};
    ImGui::PushID(label);
    
	edited |= Editor("Near clipping plane", parameter.nearClip);
	edited |= Editor("Far clipping plane", parameter.farClip);
	edited |= Editor("Field of View", parameter.fov);

    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, CameraPose& pose) {
    bool edited{};
    ImGui::PushID(label);
    
	edited |= Editor("LookAt position", pose.lookAtPos);
	edited |= Editor("Position", pose.position);
	edited |= Editor("Unk3", pose.unk3);

    ImGui::PopID();
    return edited;
}
