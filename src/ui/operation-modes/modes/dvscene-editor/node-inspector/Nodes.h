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
    using NodeFuncType = bool(*)(char*);
    using NodeType = hh::dv::DvNodeBase::NodeType;

    constexpr std::pair<NodeType, NodeFuncType> RenderNodeInspectors[] = {
        {NodeType::PATH,                RenderNodeInspector<NodeType::PATH>},
        {NodeType::CHARACTER,           RenderNodeInspector<NodeType::CHARACTER>},
        {NodeType::CHARACTER_MOTION,    RenderNodeInspector<NodeType::CHARACTER_MOTION>},
        {NodeType::MODEL,               RenderNodeInspector<NodeType::MODEL>},
        {NodeType::MODEL_MOTION,        RenderNodeInspector<NodeType::MODEL_MOTION>},
        {NodeType::MODEL_NODE,          RenderNodeInspector<NodeType::MODEL_NODE>},
        {NodeType::ELEMENT,             RenderNodeInspector<NodeType::ELEMENT>}
    };

    constexpr NodeFuncType GetNodeInspectorRender(NodeType type) {
        for (const auto& [key, func] : RenderNodeInspectors) {
            if (key == type) return func;
        }
        return nullptr;
    }
}