#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<24>(char* element) {
		bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementSpotlight::Data*>(element);
		changed |= Editor("Curve Enabled", data->curveEnabled);
		changed |= Editor("Position", data->position);
		if(data->curveEnabled)
			changed |= Editor("Finish Position", data->finishPosition);
		changed |= Editor("Rotation", data->rotation);
		float color[3]{ static_cast<float>(data->color[0]) / 255.0f, static_cast<float>(data->color[1]) / 255.0f, static_cast<float>(data->color[2]) / 255.0f };
		if (changed |= ImGui::ColorEdit3("Color", color)) {
			data->color[0] = static_cast<unsigned int>(color[0] * 255.0f);
			data->color[1] = static_cast<unsigned int>(color[1] * 255.0f);
			data->color[2] = static_cast<unsigned int>(color[2] * 255.0f);
		}
		changed |= Editor("Range", data->range);
		changed |= Editor("Intensity", data->intensity);
		changed |= Editor("Falloff", data->falloff);
		changed |= Editor("Angle", data->angle);
		return changed;
    }
}
