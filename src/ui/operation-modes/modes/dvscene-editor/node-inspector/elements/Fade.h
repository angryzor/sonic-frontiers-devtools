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
        float color[3] { 
            static_cast<float>(data->r) / 255.0f,
            static_cast<float>(data->g) / 255.0f,
            static_cast<float>(data->b) / 255.0f
        };
        if(data->enabled){
            if (changed |= ImGui::ColorEdit3("Color", color))
            {
                data->r = static_cast<int>(color[0] * 255.0f);
                data->g = static_cast<int>(color[1] * 255.0f);
                data->b = static_cast<int>(color[2] * 255.0f);
            }
        }
        return changed;
    }
}
