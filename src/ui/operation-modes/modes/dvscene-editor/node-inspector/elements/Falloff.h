#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<1045>(char* element) {
        auto* data = reinterpret_cast<app::dv::DvElementFalloff::Data*>(element);
		return Editor("Value", data->value);
    }
}
