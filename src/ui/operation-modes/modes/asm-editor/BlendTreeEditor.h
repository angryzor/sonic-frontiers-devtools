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
		bool collapseBlendSpaceNodes{ true };

		BlendTreeEditor(csl::fnd::IAllocator* allocator, hh::anim::GOCAnimator* gocAnimator, hh::anim::BlendNodeBase* focusedRootBlendNode, short focusedRootBlendNodeIndex);
		virtual void RenderContents() override;
		void RenderVariable(short variableId);
		void RenderBlendMask(short blendMaskId);
		void RenderClip(short clipId);
		void RenderState(short stateId);
		void RenderBlendSpace(short blendSpaceId);
		void RenderNode(hh::anim::BlendNodeBase* node, short nodeId);
	};
}
