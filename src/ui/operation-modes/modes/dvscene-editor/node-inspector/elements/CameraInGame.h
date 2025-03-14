#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<21>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementCameraInGame::Data*>(element);
		changed |= Editor("Unk0", data->unk0);
        return changed;
    }
}
