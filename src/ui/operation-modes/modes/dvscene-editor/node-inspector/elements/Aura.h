#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1027>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1029>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementAura*>(element);
        auto* data = elem->GetData();
		CheckboxFlags("Enabled", data->flags, app::dv::DvElementAura::Data::Flags::ENABLED);
		CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementAura::Data::Flags::CURVE_ENABLED);
        Editor("Node", data->node);
        Editor("Finish Node", data->finishNode);
    }
}
