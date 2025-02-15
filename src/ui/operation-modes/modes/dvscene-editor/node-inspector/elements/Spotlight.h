#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<24>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementSpotlight*>(element);
        auto& data = elem->binaryData;
		Editor("Curve Enabled", data.curveEnabled);
		Editor("Position", data.position);
		if(data.curveEnabled)
			Editor("Finish Position", data.finishPosition);
		Editor("Rotation", data.rotation);
		float color[3]{ static_cast<float>(data.color[0]) / 255.0f, static_cast<float>(data.color[1]) / 255.0f, static_cast<float>(data.color[2]) / 255.0f };
		if (ImGui::ColorEdit3("Color", color)) {
			data.color[0] = static_cast<unsigned int>(color[0] * 255.0f);
			data.color[1] = static_cast<unsigned int>(color[1] * 255.0f);
			data.color[2] = static_cast<unsigned int>(color[2] * 255.0f);
		}
		Editor("Range", data.range);
		Editor("Intensity", data.intensity);
		Editor("Falloff", data.falloff);
		Editor("Angle", data.angle);
    }
}
