#pragma once
#include <utilities/CompatibleObject.h>
#include <random>

namespace ui::operation_modes::modes::asm_editor {
	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;

		hh::fnd::Reference<hh::anim::GOCAnimator> gocAnimator{};
		short focusedLayer{ -1 };
		std::mt19937 mt{ std::random_device{}() };
	};
}
