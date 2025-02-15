#pragma once
#include "Timeline.h"

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_rangers
    using ElementTimelineFuncType = void(*)(Timeline*, hh::dv::DvNodeElement*);

    template<>
    void RenderElementTimeline<1>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementCameraParams*>(element->element);
		auto& elemData = elem->binaryData;
		timeline->RenderTimeline(&element->start, &element->end, elemData.curveData, 256);
    }

    template<>
    void RenderElementTimeline<7>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementCameraShakeLoop*>(element->element);
		auto& elemData = elem->binaryData;
		timeline->RenderTimeline(&element->start, &element->end, elemData.curveData, 64, false);
    }

    template<>
    void RenderElementTimeline<8>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementEffect*>(element->element);
		auto& elemData = elem->binaryData;
		timeline->RenderTimeline(&element->start, &element->end, elemData.curveData, 128);
    }

    template<>
    void RenderElementTimeline<10>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementPathInterpolation*>(element->element);
		auto& elemData = elem->binaryData;
		timeline->RenderTimeline(&element->start, &element->end, elemData.curveData, 128);
    }

    template<>
    void RenderElementTimeline<17>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementCameraOffset*>(element->element);
		auto& elemData = elem->binaryData;
		timeline->RenderTimeline(&element->start, &element->end, elemData.curveData, 256);
    }

    template<>
    void RenderElementTimeline<18>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementModelFade*>(element->element);
		auto& elemData = elem->binaryData;
		timeline->RenderTimeline(&element->start, &element->end, elemData.curveData, 128);
    }

    template<>
    void RenderElementTimeline<20>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementCameraHedgehog*>(element->element);
		auto& elemData = elem->binaryData;
		timeline->RenderTimeline(&element->start, &element->end, elemData.curveData, 32);
    }

    template<>
    void RenderElementTimeline<24>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<hh::dv::DvElementSpotlight*>(element->element);
		auto& elemData = elem->binaryData;
		timeline->RenderTimeline(&element->start, &element->end, elemData.curveData, 64);
    }

    template<>
    void RenderElementTimeline<1000>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementBloomParam*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1001>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementDOFParam*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1002>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementColorContrast*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1003>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementCameraControlParam*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1005>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementGodrayParam*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1008>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementChromaticAberrationFilterParam*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1009>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementVignetteParam*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1010>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementFade*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1011>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementLetterBox*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1017>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementTime*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1018>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementSun*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1019>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementLookAtIK*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 64);
    }

    template<>
    void RenderElementTimeline<1020>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementCameraBlurParam*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1025>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementFacialAnimation*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1027>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementAura*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1029>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementCyberSpaceNoise*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1033>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementCrossFade*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1034>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementWeather*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 32);
    }

    template<>
    void RenderElementTimeline<1036>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementVariablePointLight*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 128);
    }

    template<>
    void RenderElementTimeline<1042>(Timeline* timeline, hh::dv::DvNodeElement* element) {
        auto* elem = reinterpret_cast<app::dv::DvElementTheEndCableObject*>(element->element);
		auto* elemData = elem->GetData();
		timeline->RenderTimeline(&element->start, &element->end, elemData->curveData, 1024);
    }

    constexpr std::pair<int, ElementTimelineFuncType> RenderTimelineElements[] = {
        {1, RenderElementTimeline<1>},
        {7, RenderElementTimeline<7>},
        {8, RenderElementTimeline<8>},
        {10, RenderElementTimeline<10>},
        {17, RenderElementTimeline<17>},
        {18, RenderElementTimeline<18>},
        {20, RenderElementTimeline<20>},
        {24, RenderElementTimeline<24>},
        {1000, RenderElementTimeline<1000>},
        {1001, RenderElementTimeline<1001>},
        {1002, RenderElementTimeline<1002>},
        {1003, RenderElementTimeline<1003>},
        {1005, RenderElementTimeline<1005>},
        {1008, RenderElementTimeline<1008>},
        {1009, RenderElementTimeline<1009>},
        {1010, RenderElementTimeline<1010>},
        {1011, RenderElementTimeline<1011>},
        {1017, RenderElementTimeline<1017>},
        {1018, RenderElementTimeline<1018>},
        {1019, RenderElementTimeline<1019>},
        {1020, RenderElementTimeline<1020>},
        {1025, RenderElementTimeline<1025>},
        {1027, RenderElementTimeline<1027>},
        {1029, RenderElementTimeline<1029>},
        {1033, RenderElementTimeline<1033>},
        {1034, RenderElementTimeline<1034>},
        {1036, RenderElementTimeline<1036>},
        {1042, RenderElementTimeline<1042>}
    };

    constexpr ElementTimelineFuncType GetElementTimelineRender(int type) {
        for (const auto& [key, func] : RenderTimelineElements) {
            if (key == type) return func;
        }
        return nullptr;
    }
#endif
}
