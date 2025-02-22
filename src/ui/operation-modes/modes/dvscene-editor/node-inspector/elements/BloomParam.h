#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1000>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementBloomParam*>(element);
        auto* data = elem->GetData();
		Editor("Enabled", data->curveEnabled);
        Editor("Min Parameters", data->params);
        Editor("Max Parameters", data->finishParams);
    }
}
