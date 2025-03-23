#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<23>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementVertexAnimationTexture::Data*>(element);
		changed |= Editor("VAT Anim Name", data->vatAnimName);
		changed |= Editor("Speed", data->speed);
        return changed;
    }
}
