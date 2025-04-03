#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<17>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementCameraOffset::Data*>(element);
        bool enabled = !data->disabled;
        if (changed |= Editor("Enabled", enabled))
            data->disabled = !enabled;
        if(!data->disabled){
            changed |= Editor("Offset Position", data->offsetPosition);
            changed |= Editor("Finish Offset Position", data->finishOffsetPosition);
        }
        return changed;
    }
}
