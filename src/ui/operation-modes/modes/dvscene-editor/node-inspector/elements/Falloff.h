#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1045>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementFalloff*>(element);
        auto* data = elem->GetData();
		Editor("Value", data->value);
    }
}
