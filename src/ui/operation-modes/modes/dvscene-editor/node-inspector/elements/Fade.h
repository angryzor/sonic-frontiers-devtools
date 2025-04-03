#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::FADE>(char* element) {
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
