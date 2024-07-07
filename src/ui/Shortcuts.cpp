#include "Shortcuts.h"

ShortcutKeyCombo shortcutKeys[15]{
	{ ShortcutId::ESCAPE, ShortcutModifier_None, ImGuiKey_Escape },
	{ ShortcutId::CUT, ShortcutModifier_Ctrl, ImGuiKey_X },
	{ ShortcutId::COPY, ShortcutModifier_Ctrl, ImGuiKey_C },
	{ ShortcutId::CUT, ShortcutModifier_Ctrl, ImGuiKey_V },
	{ ShortcutId::DELETE, ShortcutModifier_None, ImGuiKey_Delete },
	{ ShortcutId::CHANGE_COORDINATE_SYSTEM, ShortcutModifier_Alt, ImGuiKey_Space },
	{ ShortcutId::TRANSLATE_MODE, ShortcutModifier_None, ImGuiKey_G },
	{ ShortcutId::ROTATE_MODE, ShortcutModifier_None, ImGuiKey_R },
	{ ShortcutId::SCALE_MODE, ShortcutModifier_None, ImGuiKey_S },
	{ ShortcutId::ONLY_X_AXIS, ShortcutModifier_None, ImGuiKey_X },
	{ ShortcutId::LOCK_X_AXIS, ShortcutModifier_Shift, ImGuiKey_X },
	{ ShortcutId::ONLY_Y_AXIS, ShortcutModifier_None, ImGuiKey_Y },
	{ ShortcutId::LOCK_Y_AXIS, ShortcutModifier_Shift, ImGuiKey_Y },
	{ ShortcutId::ONLY_Z_AXIS, ShortcutModifier_None, ImGuiKey_Z },
	{ ShortcutId::LOCK_Z_AXIS, ShortcutModifier_Shift, ImGuiKey_Z },
};

ShortcutKeyCombo& GetShortcutKeyCombo(ShortcutId shortcutId) {
	for (auto i = 0; i < sizeof(shortcutKeys) / sizeof(ShortcutKeyCombo); i++)
		if (shortcutKeys[i].shortcutId == shortcutId)
			return shortcutKeys[i];

	assert(false);

	return shortcutKeys[0];
}
