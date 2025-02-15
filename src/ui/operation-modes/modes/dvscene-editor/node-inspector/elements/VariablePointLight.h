#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    void RenderElementInspector<1036>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementVariablePointLight*>(element);
        auto* data = elem->GetData();
		Editor("Unk0", data->unk0);
        Editor("Position", data->position);
        Editor("Finish Position", data->finishPosition);
        Editor("Color", data->color);
        Editor("Finish Color", data->finishColor);
        Editor("Parameters", data->params);
        Editor("Finish Parameters", data->finishParams);
        Editor("Unk4", data->unk4);
        Editor("Unk5", data->unk5);
    }
}
