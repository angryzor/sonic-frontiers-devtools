#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1030>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementLipAnimation*>(element);
        auto* data = elem->GetData();
        Editor("Unk0", data->unk0);
        Editor("Unk1", data->unk1);
        Editor("Lip Animation Name", data->lipAnimName);
    }
}
