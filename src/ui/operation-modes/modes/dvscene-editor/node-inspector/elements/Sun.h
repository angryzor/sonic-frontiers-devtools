#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1018>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1020>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementSun::Data*>(element);
		changed |= Editor("Curve Enabled", data->curveEnabled);
        changed |= Editor("Rotation", data->rotation);
        if(data->curveEnabled)
            changed |= Editor("Finish Rotation", data->finishRotation);
        return changed;
    }
}
