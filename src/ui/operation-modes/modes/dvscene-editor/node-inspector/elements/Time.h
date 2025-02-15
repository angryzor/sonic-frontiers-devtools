#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1017>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementTime*>(element);
        auto* data = elem->GetData();
		Editor("Curve Enabled", data->curveEnabled);
        Editor("Unk1", data->unk1);
        Editor("Unk2", data->unk2);
        Editor("Unk3", data->unk3);
        Editor("Unk4", data->unk4);
        Editor("Unk5", data->unk5);
        Editor("Unk6", data->unk6);
        Editor("Unk7", data->unk7);
        Editor("Unk8", data->unk8);
    }
}
