#pragma once

bool Editor(const char* label, app_cmn::camera::CameraParameter::Target& target);
bool Editor(const char* label, app_cmn::camera::CameraParameter::Offset& offset);
bool Editor(const char* label, app_cmn::camera::CameraParameter::Orientation& orientation);
bool Editor(const char* label, app_cmn::camera::CameraParameter& parameter);
bool Editor(const char* label, app_cmn::camera::FrustumParameter& parameter);
bool Editor(const char* label, app_cmn::camera::CameraPose& pose);
