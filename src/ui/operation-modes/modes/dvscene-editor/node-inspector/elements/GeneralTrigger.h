#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1021>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1023>(char* element) {
#endif
        auto* data = reinterpret_cast<app::dv::DvElementGeneralTrigger::Data*>(element);
		return Editor("Trigger Name", data->triggerName);
    }
}
