#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* animationTypeNames[] = {
        "SKELETAL_ANIMATION",
        "UV_ANIMATION",
        "VISIBILITY_ANIMATION",
        "MATERIAL_ANIMATION",
    };

    template<>
    void RenderElementInspector<16>(hh::dv::DvElementBase* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementMultipleAnim*>(element);
        auto& data = elem->binaryData;
		Editor("Unk0", data.unk0);
		Editor("ASM State Name", data.stateName);
		Editor("Unk1", data.unk1);
		if(ImGui::TreeNode("Animations")){
            char z = 0;
            for(auto i = 0; i < data.activeAnimationsCount; i++){
                auto& x = data.animations[i];
                char buffer[68];
                snprintf(buffer, sizeof(buffer), "%s##%s", x.fileName, z);
                if(ImGui::TreeNode(buffer)){
                    int curAnmType = static_cast<int>(x.animType)-1;
                    if (ImGui::Combo("Animation Type", &curAnmType, animationTypeNames, 4))
                        x.animType = static_cast<hh::dv::DvElementMultipleAnim::Data::Animation::AnimationType>(curAnmType+1);
                    Editor("Filename", x.fileName);
                    ImGui::TreePop();
                }
                z++;
            }
            ImGui::TreePop();
        }
        if(Editor("Active Animation Count", data.activeAnimationsCount)) {
            if(data.activeAnimationsCount > 16)
                data.activeAnimationsCount = 16;
            else if(data.activeAnimationsCount < 0)
                data.activeAnimationsCount = 0;
        }
    }
}
