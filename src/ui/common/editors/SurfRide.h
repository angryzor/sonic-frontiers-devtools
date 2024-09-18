#pragma once

template<> class imgui_datatype<SurfRide::Rotation3> { public: static constexpr ImGuiDataType dtype = imgui_datatype<int>::dtype; static constexpr int components = 3; };
template<> inline float default_step<SurfRide::Rotation3>() {
	return 100.0f;
}

bool Editor(const char* label, SurfRide::Color& color);
bool Editor(const char* label, SurfRide::Colorf& color);
//bool Editor(const char* label, SurfRide::Transform& transform);

bool Editor(const char* label, SurfRide::SRS_TRS2D& transformData);
bool Editor(const char* label, SurfRide::SRS_TRS3D& transformData);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, SurfRide::SRS_IMAGECAST& imageCastData);
bool Editor(const char* label, SurfRide::SRS_REFERENCECAST& referenceCastData);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, SurfRide::SRS_SLICECAST& sliceCastData);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, SurfRide::SRS_DATA& data);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, SurfRide::SRS_USERDATA& userData);
bool Editor(const char* label, hh::fnd::ManagedResource* resource, SurfRide::SRS_USERDATA*& userData);
bool Editor(const char* label, SurfRide::SRS_TEXTDATA& textData);
