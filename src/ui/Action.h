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
	SELECTION_CHANGED,
	SET_SELECTION_TRANSFORM,
	SELECTION_TRANSFORM_CHANGED,
	OBJECTS_PICKED,
	LOCATION_PICKED,
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
	OBJECT_PLACED,
	LEVEL_EDITOR_OBJECT_TRANSFORM_CHANGED,
	LEVEL_EDITOR_CHANGING_PARAMETERS,
	LEVEL_EDITOR_STOP_CHANGING_PARAMETERS,
	LEVEL_EDITOR_SET_FOCUSED_CHUNK,
	LEVEL_EDITOR_FOCUSED_CHUNK_CHANGED,
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

class ActionDispatcher {
protected:
	static void Dispatch(const ActionBase& action);
};

class ActionProcessor {
public:
	virtual void ProcessAction(const ActionBase& action);
};
