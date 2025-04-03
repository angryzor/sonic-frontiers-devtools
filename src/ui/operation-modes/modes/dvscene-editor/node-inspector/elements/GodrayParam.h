#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1005>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1007>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementGodrayParam::Data*>(element);
        changed |= ImGui::ColorEdit3("Color", data->color);
        changed |= Editor("Intensity", data->intensity);
        return changed;
    }
}
