#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<10>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementPathInterpolation*>(element);
        auto& data = elem->binaryData;
		Editor("Use Absolute Position", data.useAbsolutePosition);
        Editor("Interpolation", data.interpolation);
        Editor("Finish Interpolation", data.finishInterpolation);
    }
}
