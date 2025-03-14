#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<1044>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementShadowShape::Data*>(element);
		changed |= CheckboxFlags("Unk0 Flag", data->flags, app::dv::DvElementShadowShape::Data::Flags::UNK0);
		changed |= CheckboxFlags("Unk1 Flag", data->flags, app::dv::DvElementShadowShape::Data::Flags::UNK1);
		changed |= CheckboxFlags("Unk2 Flag", data->flags, app::dv::DvElementShadowShape::Data::Flags::UNK2);
		changed |= CheckboxFlags("Unk3 Flag", data->flags, app::dv::DvElementShadowShape::Data::Flags::UNK3);
		changed |= CheckboxFlags("Unk4 Flag", data->flags, app::dv::DvElementShadowShape::Data::Flags::UNK4);
        changed |= Editor("Color", data->color);
        changed |= Editor("Unk0", data->unk0);
        changed |= Editor("Unk1", data->unk1);
        changed |= Editor("Unk2", data->unk2);
        changed |= Editor("Unk3", data->unk3);
        return changed;
    }
}
