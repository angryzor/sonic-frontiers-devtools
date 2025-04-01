#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<1034>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<1036>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementWeather::Data*>(element);
        unsigned int weatherType = static_cast<unsigned int>(data->weatherType);
		if(changed |= Editor("Weather Type", weatherType))
            data->weatherType = static_cast<app::dv::DvElementWeather::Data::WeatherType>(weatherType);
        return changed;
    }
}
