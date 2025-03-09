#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<6>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementCameraShake*>(element);
        auto& data = elem->binaryData;
		CheckboxFlags("Enabled", data.flags, hh::dv::DvElementCameraShake::Data::Flags::ENABLED);
		Editor("Intensity", data.intensity);
		Editor("Frequency", data.frequency);
    }
}
