#pragma once
#include <ui/common/StandaloneWindow.h>
#include "NodeEditor.h"

namespace ui::operation_modes::modes::asm_editor {
	class BlendTreeEditor : public StandaloneWindow {
	public:
		hh::fnd::Reference<hh::anim::GOCAnimator> gocAnimator{};
		hh::fnd::Reference<hh::anim::BlendNodeBase> focusedRootBlendNode{};
		short focusedRootBlendNodeIndex{ -1 };
		NodeEditor nodeEditor{ GetAllocator(), gocAnimator->asmResourceManager->animatorResource, gocAnimator };

		enum class PinType : unsigned char {
			INPUT,
			DEFAULT_TRANSITION,
			TRANSITION,
			EVENT,
		};

		BlendTreeEditor(csl::fnd::IAllocator* allocator, hh::anim::GOCAnimator* gocAnimator, hh::anim::BlendNodeBase* focusedRootBlendNode, short focusedRootBlendNodeIndex);
		virtual void RenderContents() override;
		void RenderVariable(short variableId);
		void RenderNode(hh::anim::BlendNodeBase* node, short nodeId);
	};
}
