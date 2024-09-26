#include "ASMEditor.h"
#include <ui/common/editors/Basic.h>
#include <optional>

namespace ui::operation_modes::modes::asm_editor
{
	using namespace hh::anim;

	void ASMEditor::RenderScene()
	{
		nodeEditor->Begin();

		RenderNodes();
		RenderTransitions();
		RenderFlow();

		auto& asmData = *GetContext().gocAnimator->asmResourceManager->animatorResource->binaryData;

		if (nodeEditor->ShowNodeContextMenu(ctxNodeId)) {
			ax::NodeEditor::Suspend();
			ImGui::OpenPopup("Node Context Menu");
			ax::NodeEditor::Resume();
		}

		ax::NodeEditor::Suspend();
		if (ImGui::BeginPopup("Node Context Menu")) {
			auto& state = asmData.states[ctxNodeId.idx];

			if (ImGui::MenuItem("Change to"))
				GetContext().gocAnimator->ChangeState(state.name);
			if (ImGui::MenuItem("Change to without transition"))
				GetContext().gocAnimator->ChangeStateWithoutTransition(state.name);
			ImGui::EndPopup();
		}
		ax::NodeEditor::Resume();

		nodeEditor->End();
	}

	void ASMEditor::SetGOCAnimator(GOCAnimator* gocAnimator)
	{
		GetContext().gocAnimator = gocAnimator;
		nodeEditor = new (GetAllocator()) NodeEditor{ GetAllocator(), *gocAnimator->asmResourceManager->animatorResource, gocAnimator };
	}


	void ASMEditor::RenderNodes()
	{
		auto& asmData = *GetContext().gocAnimator->asmResourceManager->animatorResource->binaryData;

		for (unsigned short i = 0; i < asmData.stateCount; i++)
			nodeEditor->State(i);
	}

	void ASMEditor::RenderTransitions()
	{
		auto& asmData = *GetContext().gocAnimator->asmResourceManager->animatorResource->binaryData;

		for (unsigned short i = 0; i < asmData.stateCount; i++) {
			auto& state = asmData.states[i];

			if (state.stateEndTransition.transitionInfo.targetStateIndex != -1)
				nodeEditor->StateDefaultTransition(i, state.stateEndTransition.transitionInfo.targetStateIndex);

			for (unsigned short j = 0; j < state.eventCount; j++)
				nodeEditor->StateEventTransition(i, asmData.events[state.eventOffset + j].transition.transitionInfo.targetStateIndex, j);

			if (state.transitionArrayIndex != -1) {
				if (nodeEditor->IsStateSelected(i)) {
					auto& transitionArray = asmData.transitionArrays[state.transitionArrayIndex];
					auto* transitions = &asmData.transitions[transitionArray.offset];

					for (int j = 0; j < transitionArray.size; j++)
						nodeEditor->StateTransition(i, transitions[j].transitionInfo.targetStateIndex);
				}
			}
		}
	}

	void ASMEditor::RenderFlow()
	{
		nodeEditor->StateActiveTransitionFlow();
	}
}
