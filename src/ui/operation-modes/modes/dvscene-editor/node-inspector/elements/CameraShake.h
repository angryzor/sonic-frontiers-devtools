#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<6>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementCameraShake::Data*>(element);
		changed |= Editor("Enabled", data->enabled);
        if(data->enabled){
            changed |= Editor("Intensity", data->intensity);
            changed |= Editor("Frequency", data->frequency);
        }
        return changed;
    }
}
