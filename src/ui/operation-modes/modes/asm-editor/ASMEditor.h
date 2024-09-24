#pragma once
#include <imgui_node_editor.h>
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"
#include <ui/common/NodeEditorAutoLayout.h>
#include "NodeEditor.h"

namespace ui::operation_modes::modes::asm_editor
{
	class ASMEditor : public OperationMode<Context>
	{
		hh::fnd::Reference<NodeEditor> nodeEditor{};
		csl::ut::MoveArray<float> outputPinTextWidths{ GetAllocator() };
		bool initialized{};

	public:
		using OperationMode<Context>::OperationMode;

		virtual void RenderScene() override;
		void SetGOCAnimator(hh::anim::GOCAnimator* gocAnimator);

	private:
		struct ActiveLayerInfo {
			hh::anim::AnimationStateMachine::LayerInfo& layer;
			hh::anim::AnimationState* prevState;
			hh::anim::AnimationState* nextState;
		};

		auto GetActiveLayers() {
			return std::views::all(GetContext().gocAnimator->animationStateMachine->layers)
				| std::views::filter([](auto& l) { return l.layerState != nullptr; })
				| std::views::transform([](auto& l) { return ActiveLayerInfo{ l, l.layerState->GetPreviousAnimationState(), l.layerState->GetNextAnimationState() }; });
		}


		void CalculateOutputPinWidths();
		ImVec4 CalculateNodeColor(hh::anim::StateData& state);
		float CalculateProgress(hh::anim::StateData& state);

		void RenderNodes();
		void RenderTransitions();
		void RenderFlow();
		void RenderFlow(short prevStateId, short nextStateId);
	};
}
