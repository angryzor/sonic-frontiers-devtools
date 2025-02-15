#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1008>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementChromaticAberrationFilterParam*>(element);
        auto* data = elem->GetData();
        Editor("Curve Enabled", data->curveEnabled);
		Editor("Parameters", data->param);
        Editor("Finish Parameters", data->finishParam);
    }
}
