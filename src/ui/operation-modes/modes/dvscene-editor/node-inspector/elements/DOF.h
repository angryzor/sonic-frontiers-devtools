#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<1047>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementDOF::Data*>(element);
		changed |= CheckboxFlags("Enabled", data->flags, app::dv::DvElementDOF::Data::Flags::ENABLED);
        if(data->flags.test(app::dv::DvElementDOF::Data::Flags::ENABLED)){
            changed |= CheckboxFlags("Use Focus Look At", data->flags, app::dv::DvElementDOF::Data::Flags::USE_FOCUS_LOOK_AT);
            changed |= CheckboxFlags("Draw Focal Plane", data->flags, app::dv::DvElementDOF::Data::Flags::DRAW_FOCAL_PLANE);
            changed |= CheckboxFlags("Lens Mode", data->flags, app::dv::DvElementDOF::Data::Flags::LENS_MODE);
            changed |= CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementDOF::Data::Flags::CURVE_ENABLED);
            changed |= Editor("Parameters", data->params);
            if(data->flags.test(app::dv::DvElementDOF::Data::Flags::CURVE_ENABLED))
                changed |= Editor("Finish Parameters", data->finishParams);
            if(ImGui::TreeNode("Blade")){
                changed |= Editor("Count", data->bladeCount);
                changed |= Editor("Curvature", data->bladeCurvature);
                changed |= Editor("Rotation", data->bladeRotation);
                ImGui::TreePop();
            }
        }
        return changed;
    }
}
