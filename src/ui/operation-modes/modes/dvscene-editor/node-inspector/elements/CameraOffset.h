#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<17>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementCameraOffset*>(element);
        auto& data = elem->binaryData;
		Editor("Enabled", data.enabled);
		Editor("Offset Position", data.offsetPosition);
		Editor("Offset Unk", data.offsetUnk);
		Editor("Unk1", data.unk1);
    }
}
