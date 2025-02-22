#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1026>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1028>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementOverrideASM*>(element);
        auto* data = elem->GetData();
		Editor("ASM Name", data->asmName);
		Editor("Target ASM Name", data->asmNameTarget);
    }
}
