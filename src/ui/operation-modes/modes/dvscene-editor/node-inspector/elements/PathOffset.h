#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<5>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementPathOffset::Data*>(element);
        csl::math::Transform transform = Affine3fToTransform(Eigen::Affine3f(data->offsetMatrix.matrix()));
        if (Editor("Transform", transform)) {
            changed |= true;
            data->offsetMatrix = Eigen::Affine3f(TransformToAffine3f(transform));
        }
        return changed;
    }
}
