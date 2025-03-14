#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1019>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1021>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementLookAtIK::Data*>(element);
		changed |= Editor("Curve Enabled", data->curveEnabled);
        changed |= Editor("Object", data->obj);
        if(data->curveEnabled)
            changed |= Editor("Finish Object", data->finishObj);
        return changed;
    }
}
