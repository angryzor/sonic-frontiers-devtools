#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1008>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1010>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementChromaticAberrationFilterParam::Data*>(element);
        changed |= Editor("Curve Enabled", data->curveEnabled);
		changed |= Editor("Parameters", data->param);
        if(data->curveEnabled)
            changed |= Editor("Finish Parameters", data->finishParam);
        return changed;
    }
}
