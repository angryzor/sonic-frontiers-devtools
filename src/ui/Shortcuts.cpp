#include "Shortcuts.h"

ShortcutDescription shortcutDescriptions[16]{
	{ "Go back", "Go back to the previous state in various situations, e.g. deselect an object." },
	{ "Cut", "Clipboard cut" },
	{ "Copy", "Clipboard copy" },
	{ "Paste", "Clipboard paste" },
	{ "Delete", "Delete selected object" },
	{ "Change Coordinate System", "Change the coordinate system" },
	{ "Translate Mode", "Switch to translate mode" },
	{ "Rotate Mode", "Switch to rotate mode" },
	{ "Scale Mode", "Switch to scale mode" },
	{ "Only X Axis", "Lock movement to the X axis" },
	{ "Lock X Axis", "Lock movement along the X axis" },
	{ "Only Y Axis", "Lock movement to the Y axis" },
	{ "Lock Y Axis", "Lock movement along the Y axis" },
	{ "Only Z Axis", "Lock movement to the Z axis" },
	{ "Lock Z Axis", "Lock movement along the Z axis" },
	{ "Toggle Place Mode", "Toggle place mode" },
};

ImGuiKeyChord shortcutBindings[16]{
	ImGuiKey_Escape,
	ImGuiMod_Ctrl | ImGuiKey_X,
	ImGuiMod_Ctrl | ImGuiKey_C,
	ImGuiMod_Ctrl | ImGuiKey_V,
	ImGuiKey_Delete,
	ImGuiMod_Alt | ImGuiKey_Space,
	ImGuiKey_G,
	ImGuiKey_R,
	ImGuiKey_S,
	ImGuiKey_X,
	ImGuiMod_Shift | ImGuiKey_X,
	ImGuiKey_Y,
	ImGuiMod_Shift | ImGuiKey_Y,
	ImGuiKey_Z,
	ImGuiMod_Shift | ImGuiKey_Z,
	ImGuiKey_CapsLock,
};

ShortcutDescription& GetShortcutDescription(ShortcutId shortcutId) {
	return shortcutDescriptions[static_cast<size_t>(shortcutId)];
}

ImGuiKeyChord GetShortcutBinding(ShortcutId shortcutId) {
	return shortcutBindings[static_cast<size_t>(shortcutId)];
}

void SetShortcutBinding(ShortcutId shortcutId, ImGuiKeyChord keyChord) {
	shortcutBindings[static_cast<size_t>(shortcutId)] = keyChord;
}
