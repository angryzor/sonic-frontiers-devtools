#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementCameraParams*>(element);
        auto& data = elem->binaryData;
        CheckboxFlags("Enabled Position", data.flags, hh::dv::DvElementCameraParams::Data::Flags::POSITION);
        CheckboxFlags("Enabled Target Position", data.flags, hh::dv::DvElementCameraParams::Data::Flags::TARGET_POSITION);
        CheckboxFlags("Enabled FOV", data.flags, hh::dv::DvElementCameraParams::Data::Flags::FOV);
        CheckboxFlags("Enabled Rotation", data.flags, hh::dv::DvElementCameraParams::Data::Flags::ROTATION);
        CheckboxFlags("Enabled Finish Position", data.flags, hh::dv::DvElementCameraParams::Data::Flags::FINISH_POSITION);
        CheckboxFlags("Enabled Finish Target Position", data.flags, hh::dv::DvElementCameraParams::Data::Flags::FINISH_TARGET_POSITION);
        CheckboxFlags("Enabled Finish FOV", data.flags, hh::dv::DvElementCameraParams::Data::Flags::FINISH_FOV);
        CheckboxFlags("Enabled Finish Rotation", data.flags, hh::dv::DvElementCameraParams::Data::Flags::FINISH_ROTATION);
        Editor("Camera Parameters", data.camera, data.flags);
        Editor("Finish Camera Parameters", data.finishCamera, data.flags, true);
    }
}
