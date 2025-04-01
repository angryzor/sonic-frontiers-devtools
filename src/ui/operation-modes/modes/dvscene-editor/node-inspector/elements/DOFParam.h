#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<1001>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementDOFParam::Data*>(element);
		changed |= CheckboxFlags("Use Focus Look At", data->flags, app::dv::DvElementDOFParam::Data::Flags::USE_FOCUS_LOOK_AT);
		changed |= CheckboxFlags("Enable Circle Dof", data->flags, app::dv::DvElementDOFParam::Data::Flags::ENABLE_CIRCLE_DOF);
		changed |= CheckboxFlags("Draw Focal Plane", data->flags, app::dv::DvElementDOFParam::Data::Flags::DRAW_FOCAL_PLANE);
		changed |= CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementDOFParam::Data::Flags::CURVE_ENABLED);
        changed |= Editor("Parameters", data->param);
        if(data->flags.test(app::dv::DvElementDOFParam::Data::Flags::CURVE_ENABLED))
            changed |= Editor("Finish Parameters", data->finishParam);
        changed |= Editor("COC Max Radius", data->cocMaxRadius);
        changed |= Editor("Focal Transition", data->focalTransition);
        changed |= Editor("Bokeh Sample Count", data->bokehSampleCount);
        changed |= Editor("Bokeh Quality", data->bokehQuality);
        changed |= Editor("Bokeh Intensity", data->bokehIntensity);
        changed |= Editor("Render Target Scale", data->rtScale);
        return changed;
    }
}
