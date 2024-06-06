#pragma once

bool ShouldCaptureKeyboardInputs();
bool ShouldCaptureMouseInputs();
void InstallInputHooks();
void ReloadInputSettings();
void AddDevToolsInputBindings(hh::hid::InputMapSettings* inputSettings);