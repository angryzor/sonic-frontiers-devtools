#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1026>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1028>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementOverrideASM::Data*>(element);
		changed |= Editor("State Name", data->asmName);
		changed |= Editor("Target State Name", data->asmNameTarget);
        return changed;
    }
}
