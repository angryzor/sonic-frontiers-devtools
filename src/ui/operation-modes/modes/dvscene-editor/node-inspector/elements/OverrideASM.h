#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1026>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementOverrideASM*>(element);
        auto* data = elem->GetData();
		Editor("ASM Name", data->asmName);
		Editor("Target ASM Name", data->asmNameTarget);
    }
}
