#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1004>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementShadowResolution*>(element);
        auto* data = elem->GetData();
		Editor("Resolution Width", data->width);
		Editor("Resolution Height", data->height);
    }
}
