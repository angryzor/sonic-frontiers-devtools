#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
	bool Editorr(const char* label, hh::dv::DvElementCameraHedgehog::Data::Camera& cam) {
		if (ImGui::TreeNode(label)) {
			Editor("Unk0", cam.unk0);
			Editor("Position", cam.position);
			Editor("Target Position", cam.targetPosition);
			Editor("Rotation", cam.rotation);
			Editor("Unk1", cam.unk1);
			Editor("Near Clip", cam.nearClip);
			Editor("Far Clip", cam.farClip);
			Editor("FOV", cam.fov);
			ImGui::TreePop();
			return true;
		}
		return false;
	}

    template<>
    void RenderElementInspector<20>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementCameraHedgehog*>(element);
        auto& data = elem->binaryData;
        Editor("Flags", data.flags);
		Editorr("Camera Before", data.camBefore);
		Editorr("Camera After", data.camAfter);
		Editor("Unk0", data.unk0);
    }
}
