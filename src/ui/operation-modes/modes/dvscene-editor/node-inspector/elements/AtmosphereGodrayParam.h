#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1006>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1008>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementAtmosphereGodrayParam::Data*>(element);
		changed |= Editor("Enabled", data->enabled);
        if(data->enabled){
            changed |= Editor("Density", data->density);
            changed |= Editor("Decay", data->decay);
            changed |= Editor("Weight", data->weight);
        }
        return changed;
    }
}
