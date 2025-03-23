#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<26>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementTexturePatternAnim::Data*>(element);
		changed |= Editor("Texture Pattern Anim Name", data->texPatAnimName);
		changed |= Editor("Speed", data->speed);
        return changed;
    }
}
