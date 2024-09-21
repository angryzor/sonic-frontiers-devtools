#pragma once
#include <utilities/CompatibleObject.h>
#include "TextureElement.h"
#include <optional>

namespace ui::operation_modes::modes::asm_editor::blend_tree_editor {
	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;
		hh::fnd::Reference<hh::anim::GOCAnimator> gocAnimator{};
		hh::anim::BlendNodeBase* focusedRootBlendNode{};
		short focusedRootBlendNodeIndex;
		std::mt19937 mt{ std::random_device{}() };
	};
}
