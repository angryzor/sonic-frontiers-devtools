#pragma once

#ifdef DELETE
#undef DELETE
#endif

enum class ShortcutId {
	ESCAPE,
	CUT,
	COPY,
	PASTE,
	DELETE,
	CHANGE_COORDINATE_SYSTEM,
	TRANSLATE_MODE,
	ROTATE_MODE,
	SCALE_MODE,
	ONLY_X_AXIS,
	LOCK_X_AXIS,
	ONLY_Y_AXIS,
	LOCK_Y_AXIS,
	ONLY_Z_AXIS,
	LOCK_Z_AXIS,
};

enum ShortcutModifier {
	ShortcutModifier_None = 0,
	ShortcutModifier_Ctrl = 1,
	ShortcutModifier_Alt = 2,
	ShortcutModifier_Shift = 4,
};

struct ShortcutKeyCombo {
	ShortcutId shortcutId;
	ShortcutModifier modifiers;
	ImGuiKey key;
};

extern ShortcutKeyCombo shortcutKeys[15];
ShortcutKeyCombo& GetShortcutKeyCombo(ShortcutId shortcutId);
