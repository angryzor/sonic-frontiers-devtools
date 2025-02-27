#include "Context.h"

namespace ui::operation_modes::modes::dvscene_editor {
	Context::Context(csl::fnd::IAllocator* allocator) : CompatibleObject { allocator }, cutsceneName { allocator } { }
}
