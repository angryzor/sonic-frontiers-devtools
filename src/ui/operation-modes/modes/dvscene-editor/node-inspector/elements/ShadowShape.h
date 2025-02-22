#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1044>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementShadowShape*>(element);
        auto* data = elem->GetData();
		CheckboxFlags("Unk0 Flag", data->flags, app::dv::DvElementShadowShape::Data::Flags::UNK0);
		CheckboxFlags("Unk1 Flag", data->flags, app::dv::DvElementShadowShape::Data::Flags::UNK1);
		CheckboxFlags("Unk2 Flag", data->flags, app::dv::DvElementShadowShape::Data::Flags::UNK2);
		CheckboxFlags("Unk3 Flag", data->flags, app::dv::DvElementShadowShape::Data::Flags::UNK3);
		CheckboxFlags("Unk4 Flag", data->flags, app::dv::DvElementShadowShape::Data::Flags::UNK4);
        Editor("Color", data->color);
        Editor("Unk0", data->unk0);
        Editor("Unk1", data->unk1);
        Editor("Unk2", data->unk2);
        Editor("Unk3", data->unk3);
    }
}
