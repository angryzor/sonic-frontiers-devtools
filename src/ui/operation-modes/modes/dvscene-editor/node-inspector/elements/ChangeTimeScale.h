#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1028>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1030>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementChangeTimeScale::Data*>(element);
        changed |= Editor("Unk Enabled", data->unkEnabled);
        changed |= Editor("Time Scale", data->timeScale);
        changed |= Editor("Unk1", data->unk1);
        changed |= Editor("Multiplier", data->multiplier);
        return changed;
    }
}
