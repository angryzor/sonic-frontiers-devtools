#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<25>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementControllerVibration::Data*>(element);
        changed |= CheckboxFlags("Ignore End", data->flags, hh::dv::DvElementControllerVibration::Data::Flags::IGNORE_END);
        changed |= Editor("Group Name", data->groupName);
        changed |= Editor("Vibration Name", data->vibrationName);
        return changed;
    }
}
