#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<14>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementVisibilityAnim::Data*>(element);
		changed |= Editor("Visibility Anim Name", data->visAnimName);
		changed |= Editor("Speed", data->speed);
        return changed;
    }
}
