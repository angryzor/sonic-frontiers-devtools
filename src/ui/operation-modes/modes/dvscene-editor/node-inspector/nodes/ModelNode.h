#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderNodeInspector<11>(char* node) {
        auto* data = reinterpret_cast<hh::dv::DvNodeModelNode::Data*>(node);
        return Editor("Model Node Name", data->nodeName);
    }
}
