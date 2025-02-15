#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1033>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementCrossFade*>(element);
        auto* data = elem->GetData();
        Editor("Curve Enabled", data->curveEnabled);
        Editor("Unk1", data->unk1);
    }
}
