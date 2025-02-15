#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1018>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementSun*>(element);
        auto* data = elem->GetData();
		Editor("Curve Enabled", data->curveEnabled);
        Editor("Rotation", data->rotation);
        Editor("Finish Rotation", data->finishRotation);
    }
}
