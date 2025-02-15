#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1023>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementDitherParam*>(element);
        auto* data = elem->GetData();
        Editor("Grass Dither Start", data->grassDitherStart);
        Editor("Grass Dither End", data->grassDitherEnd);
    }
}
