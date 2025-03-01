#include "Context.h"

namespace ui::operation_modes::modes::dvscene_editor {
    void Context::ContainsElement(bool& contains, const unsigned int& elementId, hh::dv::DvNodeBase* node)
    {
        if (contains)
            return;

        if (node->nodeType == hh::dv::DvNodeBase::NodeType::ELEMENT) {
            auto* element = reinterpret_cast<hh::dv::DvNodeElement*>(node);
            if (static_cast<unsigned int>(element->binaryData.elementId) == elementId){
                contains = true;
                return;
            }
        }

        for (auto* x : node->childrenElements0)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenElements1)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenElements2)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenPath)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenCamera)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenMotion)
            ContainsElement(contains, elementId, x);
    }

    bool Context::ContainsElement(const unsigned int& elementId)
    {
        bool contains = false;
        if (goDVSC)
            ContainsElement(contains, elementId, goDVSC->nodeTree->mainNode);
        return contains;
    }

    Context::Context(csl::fnd::IAllocator* allocator) : CompatibleObject { allocator }, cutsceneName { allocator } { }
}
