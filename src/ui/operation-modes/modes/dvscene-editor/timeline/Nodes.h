#pragma once
#include "Timeline.h"
#include "Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    using NodeTimelineFuncType = bool(*)(Timeline*, char*);

    template<>
	bool RenderNodeTimeline<4>(Timeline* timeline, char* node) {
        bool changed = false;
		auto* data = reinterpret_cast<hh::dv::DvNodeCameraMotion::Data*>(node);
        int start = data->start / 100;
        int end = data->end / 100;
        if (changed |= timeline->RenderTimeline(start, end)) {
            data->start = start * 100;
            data->end = end * 100;
        }
        return changed;
	}

	template<>
	bool RenderNodeTimeline<6>(Timeline* timeline, char* node) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvNodeCharacterMotion::Data*>(node);
        int start = data->start / 100;
        int end = data->end / 100;
        if (changed |= timeline->RenderTimeline(start, end)) {
            data->start = start * 100;
            data->end = end * 100;
        }
		return changed;
	}

	template<>
	bool RenderNodeTimeline<10>(Timeline* timeline, char* node) {
        bool changed = false;
		auto* data = reinterpret_cast<hh::dv::DvNodeModelMotion::Data*>(node);
        int start = data->start / 100;
        int end = data->end / 100;
        if (changed |= timeline->RenderTimeline(start, end)) {
            data->start = start * 100;
            data->end = end * 100;
        }
        return changed;
	}

	template<>
	bool RenderNodeTimeline<12>(Timeline* timeline, char* node) {
        bool changed = false;
		auto* data = reinterpret_cast<hh::dv::DvNodeElement::Data*>(node);
        int type = static_cast<int>(data->elementId);
        const std::pair<size_t, size_t> render = GetElementTimelineRender(data->elementId);
        
        if (render != std::pair<size_t, size_t>{})
        {
            float* curveData = &reinterpret_cast<float*>(&node[sizeof(hh::dv::DvNodeElement::Data)])[render.first / sizeof(float)];
            changed |= timeline->RenderTimeline(data->start, data->end, curveData, render.second/sizeof(float));
        }
        else
            changed |= timeline->RenderTimeline(data->start, data->end);

        return changed;
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
