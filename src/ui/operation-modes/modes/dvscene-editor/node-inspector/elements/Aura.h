#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1027>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1029>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementAura::Data*>(element);
		changed |= CheckboxFlags("Enabled", data->flags, app::dv::DvElementAura::Data::Flags::ENABLED);
        if(data->flags.test(app::dv::DvElementAura::Data::Flags::ENABLED)){
            changed |= CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementAura::Data::Flags::CURVE_ENABLED);
            changed |= Editor("Node", data->node);
            if(data->flags.test(app::dv::DvElementAura::Data::Flags::CURVE_ENABLED))
                changed |= Editor("Finish Node", data->finishNode);
        }
        return changed;
    }
}
