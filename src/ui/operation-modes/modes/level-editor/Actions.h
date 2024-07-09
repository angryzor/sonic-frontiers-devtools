#pragma once

namespace ui::operation_modes::modes::level_editor {
	using ChangingParametersAction = Action<ActionId::LEVEL_EDITOR_CHANGING_PARAMETERS>;
	using StopChangingParametersAction = Action<ActionId::LEVEL_EDITOR_STOP_CHANGING_PARAMETERS>;
	using ObjectTransformChangedAction = Action<ActionId::LEVEL_EDITOR_OBJECT_TRANSFORM_CHANGED>;
	using SetFocusedChunkAction = Action<ActionId::LEVEL_EDITOR_SET_FOCUSED_CHUNK, hh::game::ObjectWorldChunk*>;
	using FocusedChunkChangedAction = Action<ActionId::LEVEL_EDITOR_FOCUSED_CHUNK_CHANGED>;
}
