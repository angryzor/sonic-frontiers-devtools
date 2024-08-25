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
	TRANSFORM_MODE,
	TRANSLATE_MODE,
	NO_TRANSLATE_MODE,
	ROTATE_MODE,
	NO_ROTATE_MODE,
	SCALE_MODE,
	NO_SCALE_MODE,
	ONLY_X_AXIS,
	LOCK_X_AXIS,
	ONLY_Y_AXIS,
	LOCK_Y_AXIS,
	ONLY_Z_AXIS,
	LOCK_Z_AXIS,
	TOGGLE_PLACE_MODE,
};

constexpr size_t shortcutCount = 20;

struct ShortcutDescription {
	const char* name;
	const char* description;
};

extern ShortcutDescription shortcutDescriptions[shortcutCount];
extern ImGuiKeyChord shortcutBindings[shortcutCount];

ShortcutDescription& GetShortcutDescription(ShortcutId shortcutId);
ImGuiKeyChord GetShortcutBinding(ShortcutId shortcutId);
void SetShortcutBinding(ShortcutId shortcutId, ImGuiKeyChord keyChord);

template<typename F>
void ForEachShortcut(F f) {
	for (auto i = 0; i < shortcutCount; i++)
		f(static_cast<ShortcutId>(i));
}
