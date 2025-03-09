#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<3>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementDrawOff*>(element);
        auto& data = elem->binaryData;
        CheckboxFlags("Visible", data.flags, hh::dv::DvElementDrawOff::Data::Flags::VISIBLE);
        CheckboxFlags("Ignore End", data.flags, hh::dv::DvElementDrawOff::Data::Flags::IGNORE_END);
    }
}
