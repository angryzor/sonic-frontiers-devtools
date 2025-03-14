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
        changed |= Editor("Enabled", data->enabled);
        if(data->enabled){
            changed |= Editor("Time Scale", data->timeScale);
            changed |= Editor("Unk1", data->unk1);
            changed |= Editor("Unk2", data->unk2);
        }
        return changed;
    }
}
