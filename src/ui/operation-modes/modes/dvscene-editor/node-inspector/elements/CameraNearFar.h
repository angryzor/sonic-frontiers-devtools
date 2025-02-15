#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<12>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementCameraNearFar*>(element);
        auto& data = elem->binaryData;
        CheckboxFlags("Use Near Clip", data.flags, hh::dv::DvElementCameraNearFar::Data::Flags::NEAR_CLIP);
        CheckboxFlags("Use Far Clip", data.flags, hh::dv::DvElementCameraNearFar::Data::Flags::FAR_CLIP);
        if(data.flags.test(hh::dv::DvElementCameraNearFar::Data::Flags::NEAR_CLIP))
		    Editor("Near Clip", data.nearClip);
        if (data.flags.test(hh::dv::DvElementCameraNearFar::Data::Flags::FAR_CLIP))
            Editor("Far Clip", data.farClip);
    }
}
