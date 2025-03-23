#pragma once

bool Editor(const char* label, char* guid, hh::dv::DvSceneNodeTree* nodeTree);

bool SearchableCombo(const char* label, int* current_item, const char* const* items, int item_count, int searchBufSize);

bool Editor(const char* label, hh::dv::DvElementCameraHedgehog::Data::Camera& cam);
bool Editor(const char* label, hh::dv::DvElementCameraParams::Data::Camera& data);
bool Editor(const char* label, hh::dv::DvElementCameraParams::Data::Camera& data, csl::ut::Bitset<hh::dv::DvElementCameraParams::Data::Flags>& flags, bool finishParams = false);
bool Editor(const char* label, hh::dv::DvElementPathInterpolation::Data::Interpolation& data);
#ifdef DEVTOOLS_TARGET_SDK_miller
bool Editor(const char* label, hh::dv::DvElementPointLight::Data::Parameters& data);
#endif

bool Editor(const char* label, app::dv::DvElementBloomParam::Data::Parameters& data);
bool Editor(const char* label, app::dv::DvElementDOFParam::Data::DOFParam& param);
bool Editor(const char* label, app::dv::DvElementChromaticAberrationFilterParam::Data::ChromaticAberrationParam& param);
bool Editor(const char* label, app::dv::DvElementLookAtIK::Data::Object& obj);
bool Editor(const char* label, app::dv::DvElementAura::Data::AuraNode& node);
bool Editor(const char* label, app::dv::DvElementVariablePointLight::Data::Parameters& data);
#ifdef DEVTOOLS_TARGET_SDK_miller
bool Editor(const char* label, app::dv::DvElementVignetteParam::Data::ImageCircleParam& param);
bool Editor(const char* label, app::dv::DvElementCameraBlurParam::Data::BlurParameter& param);
bool Editor(const char* label, app::dv::DvElementFog::Data::FogParameter::DistanceFogParameter& param);
bool Editor(const char* label, app::dv::DvElementFog::Data::FogParameter::HeightFogParameter& param);
bool Editor(const char* label, app::dv::DvElementFog::Data::FogParameter& param);
bool Editor(const char* label, app::dv::DvElementDOF::Data::DOFParam& param);
#endif
bool Editor(const char* label, app::dv::DvElementVignetteParam::Data::VignetteParam& param);
#ifdef DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, app::dv::DvElementVignetteParam::Data::DepthParam& param);
#endif
