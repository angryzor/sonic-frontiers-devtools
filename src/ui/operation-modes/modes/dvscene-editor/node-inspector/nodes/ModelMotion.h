#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderNodeInspector<10>(char* node) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvNodeModelMotion::Data*>(node);
        changed |= CheckboxFlags("Unk0", data->flags, hh::dv::DvNodeModelMotion::Data::Flags::UNK0);
        changed |= CheckboxFlags("Update Transform", data->flags, hh::dv::DvNodeModelMotion::Data::Flags::UPDATE_TRANSFORM);
        changed |= CheckboxFlags("Use Root Bone", data->flags, hh::dv::DvNodeModelMotion::Data::Flags::USE_ROOT_BONE);
		changed |= Editor("ASM State Name", data->asmState);
		changed |= Editor("Speed", data->speed);
        return changed;
    }
}
