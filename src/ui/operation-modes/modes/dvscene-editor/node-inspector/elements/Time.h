#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1017>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1019>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementTime::Data*>(element);
		changed |= Editor("Curve Enabled", data->curveEnabled);
        changed |= Editor("Unk1", data->unk1);
        changed |= Editor("Unk2", data->unk2);
        changed |= Editor("Unk3", data->unk3);
        changed |= Editor("Unk4", data->unk4);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        changed |= Editor("Unk5", data->unk5);
        changed |= Editor("Unk6", data->unk6);
        changed |= Editor("Unk7", data->unk7);
        changed |= Editor("Unk8", data->unk8);
#endif
        return changed;
    }
}
