#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<7>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementCameraShakeLoop*>(element);
        auto& data = elem->binaryData;
		Editor("Pattern", data.pattern);
		CheckboxFlags("Enabled", data.flags, hh::dv::DvElementCameraShakeLoop::Data::Flags::ENABLED);
		Editor("Frequency", data.frequency);
		Editor("Unk4", data.unk4);
    }
}
