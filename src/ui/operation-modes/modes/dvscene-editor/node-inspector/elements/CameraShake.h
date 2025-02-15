#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<6>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementCameraShake*>(element);
        auto& data = elem->binaryData;
		Editor("Unk0", data.unk0);
		Editor("Enabled", data.enabled);
		Editor("Intensity", data.intensity);
		Editor("Frequency", data.frequency);
		Editor("Unk2", data.unk2);
		Editor("Unk3", data.unk3);
		Editor("Unk4", data.unk4);
		Editor("Unk5", data.unk5);
    }
}
