#pragma once
#include "NodeInspector.h"
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>

#include "nodes/Path.h"
#include "nodes/Character.h"
#include "nodes/CharacterMotion.h"
#include "nodes/Model.h"
#include "nodes/ModelMotion.h"
#include "nodes/ModelNode.h"
#include "nodes/Element.h"

namespace ui::operation_modes::modes::dvscene_editor {
    using NodeFuncType = void(*)(hh::dv::DvNodeBase*);

#ifdef DEVTOOLS_TARGET_SDK_rangers
    constexpr std::pair<int, NodeFuncType> RenderNodeInspectors[] = {
        {1, RenderNodeInspector<1>},
        {5, RenderNodeInspector<5>},
        {6, RenderNodeInspector<6>},
        {8, RenderNodeInspector<8>},
        {10, RenderNodeInspector<10>},
        {11, RenderNodeInspector<11>},
        {12, RenderNodeInspector<12>}
    };
#endif

    constexpr NodeFuncType GetNodeInspectorRender(int type) {
        for (const auto& [key, func] : RenderNodeInspectors) {
            if (key == type) return func;
        }
        return nullptr;
    }
}