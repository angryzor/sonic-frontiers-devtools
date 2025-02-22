#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<22>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementPointLight*>(element);
        auto& data = elem->binaryData;
        Editor("Position", data.position);
        Editor("Finish Position", data.finishPosition);
        float color[3] = { static_cast<float>(data.color[0]) / 255, static_cast<float>(data.color[1]) / 255, static_cast<float>(data.color[2]) / 255 };
        if (ImGui::ColorEdit3("Color", color))
        {
            data.color[0] = static_cast<int>(color[0] * 255);
            data.color[1] = static_cast<int>(color[1] * 255);
            data.color[2] = static_cast<int>(color[2] * 255);
        }
        float finishColor[3] = { static_cast<float>(data.finishColor[0]) / 255, static_cast<float>(data.finishColor[1]) / 255, static_cast<float>(data.finishColor[2]) / 255 };
        if (ImGui::ColorEdit3("Finish Color", finishColor))
        {
            data.finishColor[0] = static_cast<int>(finishColor[0] * 255);
            data.finishColor[1] = static_cast<int>(finishColor[1] * 255);
            data.finishColor[2] = static_cast<int>(finishColor[2] * 255);
        }
        Editor("Parameters", data.params);
        Editor("Finish Parameters", data.finishParams);
        Editor("UnkBool0", data.unkBool0);
        Editor("Unk14", data.unk14);
    }
}
