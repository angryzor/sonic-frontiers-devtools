#pragma once
#include <utilities/RflClassGenFwd.h>

struct ResourceRfls {
    static hh::fnd::RflClass resObjectWorld;
};

#ifdef DEVTOOLS_TARGET_SDK_rangers
template<> struct hh::fnd::RflClassTraits<SurfRide::SRS_PROJECT> : RflClassTraitsImpl<SurfRide::SRS_PROJECT> {};
#endif
