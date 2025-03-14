#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<1004>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementShadowResolution::Data*>(element);
		changed |= Editor("Resolution Width", data->width);
		changed |= Editor("Resolution Height", data->height);
        return changed;
    }
}
