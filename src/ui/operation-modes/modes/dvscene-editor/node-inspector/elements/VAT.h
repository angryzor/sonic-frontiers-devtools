#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1040>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementVAT*>(element);
        auto* data = elem->GetData();
		Editor("VAT Anim Name", data->vatAnimName);
		Editor("Unk1", data->unk1);
		Editor("Speed", data->speed);
		Editor("Unk3", data->unk3);
		Editor("Unk4", data->unk4);
    }
}
