#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<18>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementModelFade*>(element);
        auto& data = elem->binaryData;
		Editor("Unk0", data.unk0);
    }
}
