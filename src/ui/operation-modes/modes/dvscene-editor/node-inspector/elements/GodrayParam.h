#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1005>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementGodrayParam*>(element);
        auto* data = elem->GetData();
        ImGui::ColorEdit3("Color", data->color);
        Editor("Intensity", data->intensity);
    }
}
