#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<1003>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementCameraControlParam::Data*>(element);
		changed |= Editor("Exposure Value", data->exposureValue);
        return changed;
    }
}
