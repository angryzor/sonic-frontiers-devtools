#include <ui/input/Input.h>

HOOK(void, __fastcall, KeyboardUpdate, 0x145D51920, hh::hid::Keyboard* self)
{
	if (!ShouldCaptureKeyboardInputs()) {
		originalKeyboardUpdate(self);
	}
}

HOOK(void, __fastcall, MouseUpdate, 0x145EFCC00, hh::hid::Mouse* self)
{
	if (ShouldCaptureMouseInputs()) {
		hh::hid::Mouse::MouseState tmp = self->state;
		self->UpdateMouseState(tmp);
		self->state.deltaX = 0;
		self->state.deltaY = 0;
		self->state.cursorPosX = tmp.cursorPosX;
		self->state.cursorPosY = tmp.cursorPosY;
		self->state.prevCursorPosX = tmp.cursorPosX;
		self->state.prevCursorPosY = tmp.cursorPosY;
	}
    else {
		originalMouseUpdate(self);
	}
}

HOOK(csl::math::Vector3*, __fastcall, PointingGetTarget, 0x145CC1970, hh::hid::InputDevice* self, csl::math::Vector3* resstr, unsigned int inputId)
{
	if (ShouldCaptureMouseInputs()) {
		*resstr = csl::math::Vector3{ 0.0f, 0.0f, 0.0f };
		return resstr;
	}
	else
		return originalPointingGetTarget(self, resstr, inputId);
}

HOOK(hh::hid::InputMapSettings*, __fastcall, BindMaps, 0x1401862B0, csl::fnd::IAllocator* allocator) {
	auto* inputSettings = originalBindMaps(allocator);
    AddDevToolsInputBindings(inputSettings);
	return inputSettings;
}

void InstallInputHooks() {
	INSTALL_HOOK(BindMaps);
	INSTALL_HOOK(KeyboardUpdate);
    INSTALL_HOOK(MouseUpdate);
    INSTALL_HOOK(PointingGetTarget);
}

void ReloadInputSettings() {

}
