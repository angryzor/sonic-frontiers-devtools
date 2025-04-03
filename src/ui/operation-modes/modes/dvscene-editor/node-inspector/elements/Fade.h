#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1010>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1012>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementFade::Data*>(element);
        changed |= Editor("Enabled", data->enabled);
        if(data->enabled){
            int color[3] = { data->r, data->g, data->b };
            changed |= ColorEditor("Color", color);
        }
        return changed;
    }
}
