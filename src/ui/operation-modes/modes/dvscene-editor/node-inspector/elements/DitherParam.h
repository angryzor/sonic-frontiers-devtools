#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1023>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1025>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementDitherParam::Data*>(element);
        changed |= Editor("Grass Dither Start", data->grassDitherStart);
        changed |= Editor("Grass Dither End", data->grassDitherEnd);
        return changed;
    }
}
