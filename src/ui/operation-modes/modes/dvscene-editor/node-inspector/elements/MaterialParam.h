#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<27>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementMaterialParam*>(element);
        auto& data = elem->binaryData;
		Editor("Material Name", data.materialName);
		Editor("Parameter Name", data.paramName);
		CheckboxFlags("Use Parameter X", data.flags, hh::dv::DvElementMaterialParam::Data::Flags::PARAM_X);
		CheckboxFlags("Use Parameter Y", data.flags, hh::dv::DvElementMaterialParam::Data::Flags::PARAM_Y);
		CheckboxFlags("Use Parameter Z", data.flags, hh::dv::DvElementMaterialParam::Data::Flags::PARAM_Z);
		CheckboxFlags("Use Parameter W", data.flags, hh::dv::DvElementMaterialParam::Data::Flags::PARAM_W);
        if(data.flags.test(hh::dv::DvElementMaterialParam::Data::Flags::PARAM_X))
            Editor("Parameter X", data.params[0]);
        if(data.flags.test(hh::dv::DvElementMaterialParam::Data::Flags::PARAM_Y))
            Editor("Parameter Y", data.params[1]);
        if(data.flags.test(hh::dv::DvElementMaterialParam::Data::Flags::PARAM_Z))
            Editor("Parameter Z", data.params[2]);
        if(data.flags.test(hh::dv::DvElementMaterialParam::Data::Flags::PARAM_W))
            Editor("Parameter W", data.params[3]);
        if(data.flags.test(hh::dv::DvElementMaterialParam::Data::Flags::PARAM_X))
            Editor("Finish Parameter X", data.finishParams[0]);
        if(data.flags.test(hh::dv::DvElementMaterialParam::Data::Flags::PARAM_Y))
            Editor("Finish Parameter Y", data.finishParams[1]);
        if(data.flags.test(hh::dv::DvElementMaterialParam::Data::Flags::PARAM_Z))
            Editor("Finish Parameter Z", data.finishParams[2]);
        if(data.flags.test(hh::dv::DvElementMaterialParam::Data::Flags::PARAM_W))
            Editor("Finish Parameter W", data.finishParams[3]);
    }
}
