#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderNodeInspector<11>(hh::dv::DvNodeBase* node) {
        auto* mdlNode = reinterpret_cast<hh::dv::DvNodeModelNode*>(node);
        auto& data = mdlNode->binaryData;
        Editor("Model Node Name", data.nodeName);
    }
}
