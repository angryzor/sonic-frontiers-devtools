#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1030>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1032>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementLipAnimation*>(element);
        auto* data = elem->GetData();
        Editor("Unk0", data->unk0);
        Editor("Unk1", data->unk1);
        Editor("Lip Animation Name", data->lipAnimName);
    }
}
