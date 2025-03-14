#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<1041>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementQTEAccel::Data*>(element);
		changed |= Editor("Unk0", data->unk0);
		changed |= Editor("Unk1", data->unk1);
		changed |= Editor("Unk2", data->dword8);
		changed |= Editor("Unk3", data->dwordC);
        return changed;
    }
}
