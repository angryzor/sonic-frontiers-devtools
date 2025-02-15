#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1001>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementDOFParam*>(element);
        auto* data = elem->GetData();
		CheckboxFlags("Use Focus Look At", data->flags, app::dv::DvElementDOFParam::Data::Flags::USE_FOCUS_LOOK_AT);
		CheckboxFlags("Enable Circle Dof", data->flags, app::dv::DvElementDOFParam::Data::Flags::ENABLE_CIRCLE_DOF);
		CheckboxFlags("Draw Focal Plane", data->flags, app::dv::DvElementDOFParam::Data::Flags::DRAW_FOCAL_PLANE);
		CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementDOFParam::Data::Flags::UNK0);
        Editor("Parameters", data->param);
        Editor("Finish Parameters", data->finishParam);
        Editor("COC Max Radius", data->cocMaxRadius);
        Editor("Focal Transition", data->focalTransition);
        Editor("Bokeh Sample Count", data->bokehSampleCount);
        Editor("Bokeh Quality", data->bokehQuality);
        Editor("Bokeh Intensity", data->bokehIntensity);
        Editor("Render Target Scale", data->rtScale);
    }
}
