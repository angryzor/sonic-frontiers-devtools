#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::OVERRIDE_ASM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementOverrideASM::Data*>(element);
		changed |= Editor("State Name", data->asmName);
		changed |= Editor("Target State Name", data->asmNameTarget);
        return changed;
    }
}
