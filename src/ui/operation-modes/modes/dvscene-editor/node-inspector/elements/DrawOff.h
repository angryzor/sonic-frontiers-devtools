#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<3>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementDrawOff::Data*>(element);
        changed |= CheckboxFlags("Visible", data->flags, hh::dv::DvElementDrawOff::Data::Flags::VISIBLE);
        changed |= CheckboxFlags("Ignore End", data->flags, hh::dv::DvElementDrawOff::Data::Flags::IGNORE_END);
        return changed;
    }
}
