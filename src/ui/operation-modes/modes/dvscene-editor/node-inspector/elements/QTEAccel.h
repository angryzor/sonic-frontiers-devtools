#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* QTEAccelButtonNames[]{
        "A",
        "B",
        "X",
        "Y"
    };

    template<>
    bool RenderElementInspector<1041>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementQTEAccel::Data*>(element);
        int curBtnType = static_cast<int>(data->button);
        if (changed |= ImGui::Combo("Button", &curBtnType, QTEAccelButtonNames, 4))
            data->button = static_cast<app::dv::DvElementQTEAccel::Data::Button>(curBtnType);
		changed |= Editor("Unk1", data->unk1);
		changed |= Editor("Unk2", data->dword8);
		changed |= Editor("Unk3", data->dwordC);
        changed |= Editor("Sound Name", data->soundName);
        return changed;
    }
}
