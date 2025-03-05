#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<5>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementPathOffset*>(element);
        auto& data = elem->binaryData;
        csl::math::Transform transform = Affine3fToTransform(Eigen::Affine3f(data.offsetMatrix.matrix()));
        if (Editor("Transform", transform))
            data.offsetMatrix = Eigen::Affine3f(TransformToAffine3f(transform));
    }
}
