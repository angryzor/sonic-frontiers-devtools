#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderNodeInspector<8>(hh::dv::DvNodeBase* node) {
        auto* mdlNode = reinterpret_cast<hh::dv::DvNodeModel*>(node);
        auto& data = mdlNode->binaryData;
		Editor("Model Name", data.modelName);
		Editor("Skeleton Name", data.skeletonName);
		Editor("Use Internal Name", data.useName3);
		if (data.useName3)
			Editor("Internal Name", data.name3);
    }
}
