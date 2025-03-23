#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<15>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementMaterialAnim::Data*>(element);
		changed |= Editor("Material Anim Name", data->matAnimName);
		changed |= Editor("Speed", data->speed);
        return changed;
    }
}
