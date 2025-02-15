#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1021>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementGeneralTrigger*>(element);
        auto* data = elem->GetData();
		Editor("Trigger Name", data->triggerName);
    }
}
