#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderNodeInspector<6>(hh::dv::DvNodeBase* node) {
        auto* charMotNode = reinterpret_cast<hh::dv::DvNodeCharacterMotion*>(node);
        auto& data = charMotNode->binaryData;
		Editor("ASM State Name", data.asmState);
		Editor("Speed", data.speed);
    }
}
