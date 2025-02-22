#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1043>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementObjectTimeStop*>(element);
        auto* data = elem->GetData();
		Editor("Type", data->type);
    }
}
