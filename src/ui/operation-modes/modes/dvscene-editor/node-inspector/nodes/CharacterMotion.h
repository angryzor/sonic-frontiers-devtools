#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderNodeInspector<6>(char* node) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvNodeCharacterMotion::Data*>(node);
		changed |= Editor("ASM State Name", data->asmState);
		changed |= Editor("Speed", data->speed);
        return changed;
    }
}
