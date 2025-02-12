#pragma once

template<> class imgui_datatype<ucsl::resources::swif::v6::Rotation3> { public: static constexpr ImGuiDataType dtype = imgui_datatype<int>::dtype; static constexpr int components = 3; };
template<> inline float default_step<ucsl::resources::swif::v6::Rotation3>() {
	return 100.0f;
}

bool Editor(const char* label, ucsl::resources::swif::v6::Color& color);
bool Editor(const char* label, ucsl::resources::swif::v6::Colorf& color);
//bool Editor(const char* label, ucsl::resources::swif::v6::Transform& transform);

bool Editor(const char* label, ucsl::resources::swif::v6::SRS_TRS2D& transformData);
bool Editor(const char* label, ucsl::resources::swif::v6::SRS_TRS3D& transformData);
bool Editor(const char* label, ucsl::resources::swif::v6::SRS_CASTNODE& castData);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::v6::SRS_ANIMATION& animation);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::v6::SRS_IMAGECAST& imageCastData);
bool Editor(const char* label, ucsl::resources::swif::v6::SRS_REFERENCECAST& referenceCastData);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::v6::SRS_SLICECAST& sliceCastData);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::v6::SRS_DATA& data);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::v6::SRS_USERDATA& userData);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, ucsl::resources::swif::v6::SRS_USERDATA*& userData);
bool Editor(const char* label, ucsl::resources::swif::v6::SRS_TEXTDATA& textData);
