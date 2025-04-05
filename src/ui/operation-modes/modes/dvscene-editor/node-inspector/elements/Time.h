#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::TIME>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementTime::Description*>(element);
		changed |= Editor("Curve Enabled", data->curveEnabled);
        changed |= Editor("Time", data->time);
        if(data->curveEnabled)
            changed |= Editor("Finish Time", data->finishTime);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        changed |= Editor("Unk5", data->unk5);
        changed |= Editor("Unk6", data->unk6);
        changed |= Editor("Unk7", data->unk7);
        changed |= Editor("Unk8", data->unk8);
#endif
        return changed;
    }
}
