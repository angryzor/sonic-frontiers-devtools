#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<23>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementVertexAnimationTexture::Data*>(element);
		changed |= Editor("VAT Anim Name", data->vatAnimName);
		changed |= Editor("Unk1", data->unk1);
		changed |= Editor("Speed", data->speed);
		changed |= Editor("Unk3", data->unk3);
		changed |= Editor("Unk4", data->unk4);
        return changed;
    }
}
