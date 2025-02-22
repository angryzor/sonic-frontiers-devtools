#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1023>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1025>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementDitherParam*>(element);
        auto* data = elem->GetData();
        Editor("Grass Dither Start", data->grassDitherStart);
        Editor("Grass Dither End", data->grassDitherEnd);
    }
}
