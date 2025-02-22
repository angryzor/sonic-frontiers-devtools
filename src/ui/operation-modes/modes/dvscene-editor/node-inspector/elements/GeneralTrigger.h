#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1021>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1023>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementGeneralTrigger*>(element);
        auto* data = elem->GetData();
		Editor("Trigger Name", data->triggerName);
    }
}
