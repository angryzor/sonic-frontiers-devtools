#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* transTypeNames[] = {
        "NODE",
        "NODE_AND_FRAME",
        "NODE_POSITION"
    };

    template<>
    void RenderElementInspector<8>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementEffect*>(element);
        auto& data = elem->binaryData;
        csl::math::Transform transform = Affine3fToTransform(Eigen::Affine3f(data.effectMatrix.matrix()));
        if (Editor("Effect Transform", transform))
            data.effectMatrix = Eigen::Affine3f(TransformToAffine3f(transform));
        Editor("Effect Name", data.effectName);
        CheckboxFlags("Unk0", data.flags, hh::dv::DvElementEffect::Data::Flags::UNK0);
        CheckboxFlags("Quaternion", data.flags, hh::dv::DvElementEffect::Data::Flags::QUATERNION);
        CheckboxFlags("Unk1", data.flags, hh::dv::DvElementEffect::Data::Flags::UNK1);
        CheckboxFlags("Model Space Node", data.flags, hh::dv::DvElementEffect::Data::Flags::MODEL_SPACE_NODE);
        Editor("Eff Create Info Unk1a", data.effCreateInfoUnk1A);
        CheckboxFlags("Unk0 flags2", data.flags2, hh::dv::DvElementEffect::Data::Flags2::UNK0);
        CheckboxFlags("Persistent", data.flags2, hh::dv::DvElementEffect::Data::Flags2::PERSISTENT);
        float color[] = { static_cast<float>(data.r)/255.0f, static_cast<float>(data.g) / 255.0f, static_cast<float>(data.b) / 255.0f, static_cast<float>(data.a) / 255.0f };
        if (ImGui::ColorEdit4("Effect Color", color)) {
            data.r = static_cast<unsigned char>(color[0] * 255.0f);
            data.g = static_cast<unsigned char>(color[1] * 255.0f);
            data.b = static_cast<unsigned char>(color[2] * 255.0f);
            data.a = static_cast<unsigned char>(color[3] * 255.0f);
        }
        Editor("Eff Create Info Unk5a", data.effCreateInfoUnk5A);
        Editor("Eff Create Info Unk5", data.effCreateInfoUnk5);
        int curTransType = static_cast<int>(data.effTransType);
        if (ImGui::Combo("Eff Trans Type", &curTransType, transTypeNames, 3))
            data.effTransType = static_cast<hh::dv::DvElementEffect::Data::EffectTransType>(curTransType);
        Editor("Unk7", data.unk7);
        Editor("Unk8", data.unk8);
    }
}
