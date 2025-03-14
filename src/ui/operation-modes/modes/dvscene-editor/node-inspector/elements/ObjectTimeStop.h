#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<1043>(char* element) {
        auto* data = reinterpret_cast<app::dv::DvElementObjectTimeStop::Data*>(element);
		return Editor("Type", data->type);
    }
}
