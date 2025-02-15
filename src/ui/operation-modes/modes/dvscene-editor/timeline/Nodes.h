#pragma once
#include "Timeline.h"
#include "Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    using NodeTimelineFuncType = void(*)(Timeline*, hh::dv::DvNodeBase*);

#ifdef DEVTOOLS_TARGET_SDK_rangers
    template<>
	void RenderNodeTimeline<4>(Timeline* timeline, hh::dv::DvNodeBase* node) {
		auto* camMot = reinterpret_cast<hh::dv::DvNodeCameraMotion*>(node);
        auto& data = camMot->binaryData;
		timeline->RenderTimeline(data.start, data.end);
	}

	template<>
	void RenderNodeTimeline<6>(Timeline* timeline, hh::dv::DvNodeBase* node) {
		auto* charMot = reinterpret_cast<hh::dv::DvNodeCharacterMotion*>(node);
        auto& data = charMot->binaryData;
		timeline->RenderTimeline(data.start, data.end);
	}

	template<>
	void RenderNodeTimeline<10>(Timeline* timeline, hh::dv::DvNodeBase* node) {
		auto* modelMot = reinterpret_cast<hh::dv::DvNodeModelMotion*>(node);
        auto& data = modelMot->binaryData;
		timeline->RenderTimeline(data.start, data.end);
	}

	template<>
	void RenderNodeTimeline<12>(Timeline* timeline, hh::dv::DvNodeBase* node) {
		auto* element = reinterpret_cast<hh::dv::DvNodeElement*>(node);
        auto& data = element->binaryData;
        int type = static_cast<int>(data.elementId);
        ElementTimelineFuncType render;
        render = GetElementTimelineRender(type);
        
        if(render)
            render(timeline, element);
        else
            timeline->RenderTimeline(element->start, element->end);
	}
#endif

    constexpr std::pair<int, NodeTimelineFuncType> RenderTimelineNodes[] = {
#ifdef DEVTOOLS_TARGET_SDK_rangers
        {4, RenderNodeTimeline<4>},
        {6, RenderNodeTimeline<6>},
        {10, RenderNodeTimeline<10>},
        {12, RenderNodeTimeline<12>}
#endif
#ifdef DEVTOOLS_TARGET_SDK_miller
        {}
#endif
    };

    constexpr NodeTimelineFuncType GetNodeTimelineRender(int type) {
        for (const auto& [key, func] : RenderTimelineNodes) {
            if (key == type) return func;
        }
        return nullptr;
    }
}
