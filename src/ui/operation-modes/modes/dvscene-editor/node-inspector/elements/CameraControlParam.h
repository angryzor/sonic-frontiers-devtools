#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1003>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementCameraControlParam*>(element);
        auto* data = elem->GetData();
		Editor("Exposure Value", data->exposureValue);
    }
}
