#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderNodeInspector<1>(hh::dv::DvNodeBase* node) {
        auto* pathNode = reinterpret_cast<hh::dv::DvNodePath*>(node);
        Editor("Transform", pathNode->transform);
    }
}
