#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    void RenderElementInspector<1034>(hh::dv::DvElementBase* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    void RenderElementInspector<1036>(hh::dv::DvElementBase* element) {
#endif
        auto* elem = reinterpret_cast<app::dv::DvElementWeather*>(element);
        auto* data = elem->GetData();
        unsigned int weatherType = static_cast<unsigned int>(data->weatherType);
		if(Editor("Weather Type", weatherType))
            data->weatherType = static_cast<app::dv::DvElementWeather::Data::WeatherType>(weatherType);
    }
}
