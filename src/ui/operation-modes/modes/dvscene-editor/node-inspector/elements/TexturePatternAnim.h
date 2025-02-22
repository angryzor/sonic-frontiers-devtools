#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<26>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementTexturePatternAnim*>(element);
        auto& data = elem->binaryData;
		Editor("Texture Pattern Anim Name", data.texPatAnimName);
		Editor("Unk1", data.unk1);
		Editor("Speed", data.speed);
		Editor("Unk3", data.unk3);
		Editor("Unk4", data.unk4);
    }
}
