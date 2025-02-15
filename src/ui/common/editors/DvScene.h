#pragma once

#ifdef DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, app::dv::DvElementDOFParam::Data::DOFParam& param);
bool Editor(const char* label, app::dv::DvElementChromaticAberrationFilterParam::Data::ChromaticAberrationParam& param);
bool Editor(const char* label, app::dv::DvElementVignetteParam::Data::VignetteParam& param);
bool Editor(const char* label, app::dv::DvElementVignetteParam::Data::MinMaxParam& param);
bool Editor(const char* label, app::dv::DvElementLookAtIK::Data::Object& obj);
bool Editor(const char* label, app::dv::DvElementAura::Data::AuraNode& node);
bool Editor(const char* label, hh::dv::DvElementCameraParams::Data::Camera& data);
bool Editor(const char* label, hh::dv::DvElementCameraParams::Data::Camera& data, csl::ut::Bitset<hh::dv::DvElementCameraParams::Data::Flags>& flags, bool finishParams = false);
bool Editor(const char* label, hh::dv::DvElementPathInterpolation::Data::Interpolation& data);
bool Editor(const char* label, app::dv::DvElementVariablePointLight::Data::Parameters& data);
bool Editor(const char* label, app::dv::DvElementBloomParam::Data::Parameters& data);
#endif