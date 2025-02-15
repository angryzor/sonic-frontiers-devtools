#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderNodeInspector<5>(hh::dv::DvNodeBase* node) {
        auto* charNode = reinterpret_cast<hh::dv::DvNodeCharacter*>(node);
        auto& data = charNode->binaryData;
		Editor("Model Name", data.modelName);
		Editor("Skeleton Name", data.skeletonName);
		Editor("Use Internal Name", data.useName3);
		if (data.useName3)
			Editor("Internal Name", data.name3);
    }
}
