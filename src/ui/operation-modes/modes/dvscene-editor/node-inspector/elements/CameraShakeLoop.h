#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<7>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementCameraShakeLoop*>(element);
        auto& data = elem->binaryData;
		Editor("Unk0", data.unk0);
		CheckboxFlags("Enabled", data.flags, hh::dv::DvElementCameraShakeLoop::Data::Flags::ENABLED);
		Editor("Frequency", data.frequency);
		Editor("Unk3", data.unk3);
		Editor("Unk4", data.unk4);
		Editor("Unk5", data.unk5);
		Editor("Unk6", data.unk6);
		Editor("Unk7", data.unk7);
    }
}
