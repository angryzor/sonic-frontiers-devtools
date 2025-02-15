#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1027>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementAura*>(element);
        auto* data = elem->GetData();
		CheckboxFlags("Enabled", data->flags, app::dv::DvElementAura::Data::Flags::ENABLED);
		CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementAura::Data::Flags::CURVE_ENABLED);
        Editor("Node", data->node);
        Editor("Finish Node", data->finishNode);
    }
}
