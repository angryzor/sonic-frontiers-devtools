#pragma once
#include "Timeline.h"
#include "Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    using NodeTimelineFuncType = void(*)(Timeline*, hh::dv::DvNodeBase*);

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
        const std::pair<size_t, size_t> render = GetElementTimelineRender(type);
        
        if (render == std::pair<size_t, size_t>{})
        {
            float* curveData;
            if (type >= 1000) {
                auto* elem = reinterpret_cast<app::dv::AppDvElementBase*>(element->element);
                auto* elemData = reinterpret_cast<const float*>(elem->GetBinaryData());
                curveData = const_cast<float*>(&elemData[render.first/4]);
            }
            else
                curveData = &reinterpret_cast<float*>(element->element)[(render.first + sizeof(hh::dv::DvElementBase)) / 4];
            timeline->RenderTimeline(element->start, element->end, curveData, render.second);
        }
        else
            timeline->RenderTimeline(element->start, element->end);
	}

    constexpr std::pair<int, NodeTimelineFuncType> RenderTimelineNodes[] = {
        {4, RenderNodeTimeline<4>},
        {6, RenderNodeTimeline<6>},
        {10, RenderNodeTimeline<10>},
        {12, RenderNodeTimeline<12>}
    };

    constexpr NodeTimelineFuncType GetNodeTimelineRender(int type) {
        for (const auto& [key, func] : RenderTimelineNodes) {
            if (key == type) return func;
        }
        return nullptr;
    }
}
