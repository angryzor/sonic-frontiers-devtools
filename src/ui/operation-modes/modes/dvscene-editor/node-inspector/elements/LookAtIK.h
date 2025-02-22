#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1019>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1021>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementLookAtIK*>(element);
        auto* data = elem->GetData();
		Editor("Curve Enabled", data->curveEnabled);
        Editor("Object", data->obj);
        Editor("Finish Object", data->finishObj);
    }
}
