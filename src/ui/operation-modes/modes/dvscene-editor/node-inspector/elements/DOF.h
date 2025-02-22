#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1047>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementDOF*>(element);
        auto* data = elem->GetData();
		CheckboxFlags("Enabled", data->flags, app::dv::DvElementDOF::Data::Flags::ENABLED);
		CheckboxFlags("Use Focus Look At", data->flags, app::dv::DvElementDOF::Data::Flags::USE_FOCUS_LOOK_AT);
		CheckboxFlags("Draw Focal Plane", data->flags, app::dv::DvElementDOF::Data::Flags::DRAW_FOCAL_PLANE);
		CheckboxFlags("Lens Mode", data->flags, app::dv::DvElementDOF::Data::Flags::LENS_MODE);
		CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementDOF::Data::Flags::CURVE_ENABLED);
        Editor("Parameters", data->params);
        Editor("Finish Parameters", data->finishParams);
        if(ImGui::TreeNode("Blade")){
            Editor("Count", data->bladeCount);
            Editor("Curvature", data->bladeCurvature);
            Editor("Rotation", data->bladeRotation);
            ImGui::TreePop();
        }
    }
}
