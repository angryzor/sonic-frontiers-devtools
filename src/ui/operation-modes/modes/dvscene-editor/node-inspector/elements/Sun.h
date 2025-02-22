#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1018>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1020>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementSun*>(element);
        auto* data = elem->GetData();
		Editor("Curve Enabled", data->curveEnabled);
        Editor("Rotation", data->rotation);
        Editor("Finish Rotation", data->finishRotation);
    }
}
