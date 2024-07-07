#pragma once
#include <utilities/CompatibleObject.h>

#ifdef DELETE
#undef DELETE
#endif

enum class ActionId {
	SELECT,
	DESELECT,
	SELECT_ALL,
	DESELECT_ALL,
	SET_SELECTION_TRANSFORM,
	COPY,
	PASTE,
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
	DELETE,
};

struct ActionBase {
	ActionId id;
};

struct EmptyActionPayload {};

template<ActionId actionId, typename Payload = EmptyActionPayload>
struct Action : ActionBase {
	typedef Payload PayloadType;

	static constexpr ActionId id = actionId;

	Payload payload;

	explicit Action() : ActionBase{ actionId }, payload{} {}
	explicit Action(const Payload& payload) : ActionBase{ actionId }, payload{ payload } {}
	explicit Action(Payload&& payload) : ActionBase{ actionId }, payload{ std::move(payload) } {}
};
