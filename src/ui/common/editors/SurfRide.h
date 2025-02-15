#pragma once

template<> class imgui_datatype<ucsl::resources::swif::swif_version::Rotation3> { public: static constexpr ImGuiDataType dtype = imgui_datatype<int>::dtype; static constexpr int components = 3; };
template<> inline float default_step<ucsl::resources::swif::swif_version::Rotation3>() {
	return 100.0f;
}

bool Editor(const char* label, ucsl::resources::swif::swif_version::Color& color);
bool Editor(const char* label, ucsl::resources::swif::swif_version::Colorf& color);
//bool Editor(const char* label, ucsl::resources::swif::swif_version::Transform& transform);

bool Editor(const char* label, ucsl::resources::swif::swif_version::SRS_TRS2D& transformData);
bool Editor(const char* label, ucsl::resources::swif::swif_version::SRS_TRS3D& transformData);
bool Editor(const char* label, ucsl::resources::swif::swif_version::SRS_CASTNODE& castData);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::swif_version::SRS_ANIMATION& animation);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::swif_version::SRS_IMAGECAST& imageCastData);
bool Editor(const char* label, ucsl::resources::swif::swif_version::SRS_REFERENCECAST& referenceCastData);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::swif_version::SRS_SLICECAST& sliceCastData);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::swif_version::SRS_DATA& data);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::swif_version::SRS_USERDATA& userData);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::swif_version::SRS_USERDATA*& userData);
bool Editor(const char* label, ucsl::resources::swif::swif_version::SRS_TEXTDATA& textData);
