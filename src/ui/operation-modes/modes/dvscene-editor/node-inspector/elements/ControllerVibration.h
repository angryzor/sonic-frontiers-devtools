#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<25>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementControllerVibration*>(element);
        auto& data = elem->binaryData;
#ifdef DEVTOOLS_TARGET_SDK_miller
        Editor("Group Name", data.groupName);
#endif
        Editor("Vibration Name", data.vibrationName);
    }
}
