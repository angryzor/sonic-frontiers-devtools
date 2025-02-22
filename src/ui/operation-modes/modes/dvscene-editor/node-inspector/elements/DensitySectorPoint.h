#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1038>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1040>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementDensitySectorPoint*>(element);
        auto* data = elem->GetData();
        Editor("Unk0", data->unk0);
        Editor("Unk1", data->unk1);
        Editor("Unk2", data->unk2);
    }
}
