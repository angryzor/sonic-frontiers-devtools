#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1038>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1040>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementDensitySectorPoint::Data*>(element);
        changed |= Editor("Unk0", data->unk0);
        changed |= Editor("Unk1", data->unk1);
        changed |= Editor("Unk2", data->unk2);
        return changed;
    }
}
