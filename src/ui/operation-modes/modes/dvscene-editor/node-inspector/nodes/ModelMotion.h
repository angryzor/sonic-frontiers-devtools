#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderNodeInspector<10>(hh::dv::DvNodeBase* node) {
        auto* mdlMotNode = reinterpret_cast<hh::dv::DvNodeModelMotion*>(node);
        auto& data = mdlMotNode->binaryData;
		Editor("ASM State Name", data.asmState);
		Editor("Speed", data.speed);
    }
}
