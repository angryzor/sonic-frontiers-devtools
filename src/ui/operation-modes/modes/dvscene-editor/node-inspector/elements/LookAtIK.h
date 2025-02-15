#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1019>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementLookAtIK*>(element);
        auto* data = elem->GetData();
		Editor("Curve Enabled", data->curveEnabled);
        Editor("Object", data->obj);
        Editor("Finish Object", data->finishObj);
    }
}
