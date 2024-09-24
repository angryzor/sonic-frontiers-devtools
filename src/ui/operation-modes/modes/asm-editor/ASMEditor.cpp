#include "ASMEditor.h"
#include <ui/common/editors/Basic.h>
#include <optional>

namespace ui::operation_modes::modes::asm_editor
{
	using namespace hh::anim;

	const char* stateTypeNames[]{ "Clip", "Blend tree", "None" };

	void ASMEditor::RenderScene()
	{
		if (!initialized) {
			CalculateOutputPinWidths();
			initialized = true;
		}

		ImPlot::PushColormap(ImPlotColormap_Deep);
		nodeEditor->Begin();

		RenderNodes();
		RenderTransitions();
		RenderFlow();

		nodeEditor->End();
		ImPlot::PopColormap();
	}

	void ASMEditor::SetGOCAnimator(GOCAnimator* gocAnimator)
	{
		GetContext().gocAnimator = gocAnimator;
		nodeEditor = new (GetAllocator()) NodeEditor{ GetAllocator(), *gocAnimator->asmResourceManager->animatorResource, gocAnimator };
	}

	void ASMEditor::CalculateOutputPinWidths()
	{
		auto& asmData = *GetContext().gocAnimator->asmResourceManager->animatorResource->binaryData;

		for (unsigned short i = 0; i < asmData.stateCount; i++) {
			auto& state = asmData.states[i];
			float maxTextWidth = 0.0f;
			for (unsigned short j = 0; j < state.eventCount; j++) {
				auto& event = asmData.events[state.eventOffset + j];
				maxTextWidth = std::fmaxf(maxTextWidth, ImGui::CalcTextSize(event.name).x);
			}
			outputPinTextWidths.push_back(maxTextWidth);
		}
	}

	ImVec4 ASMEditor::CalculateNodeColor(StateData& state)
	{
		for (auto l : GetActiveLayers()) {
			auto layerColor = ImPlot::GetColormapColor(l.layer.layerId);

			if (l.nextState && l.nextState->stateData == &state)
				return layerColor;
			else if (l.prevState && l.prevState->stateData == &state)
				return { layerColor.x * 0.7f, layerColor.y * 0.7f, layerColor.z * 0.7f, 1.0f };
		}

		return ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeBorder];
	}

	float ASMEditor::CalculateProgress(StateData& state)
	{
		for (auto l : GetActiveLayers()) {
			if (l.nextState && l.nextState->stateData == &state)
				return l.nextState->implementation.currentTime / l.nextState->implementation.duration;
			else if (l.prevState && l.prevState->stateData == &state)
				return l.prevState->implementation.currentTime / l.prevState->implementation.duration;
		}

		return 0.0f;
	}

	void ASMEditor::RenderNodes()
	{
		auto& asmData = *GetContext().gocAnimator->asmResourceManager->animatorResource->binaryData;

		for (unsigned short i = 0; i < asmData.stateCount; i++) {
			auto& state = asmData.states[i];

			ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, CalculateNodeColor(state));
			nodeEditor->State(i, CalculateNodeColor(state), CalculateProgress(state));
			ax::NodeEditor::PopStyleColor();
		}
	}

	void ASMEditor::RenderTransitions()
	{
		auto& asmData = *GetContext().gocAnimator->asmResourceManager->animatorResource->binaryData;

		for (unsigned short i = 0; i < asmData.stateCount; i++) {
			auto& state = asmData.states[i];
			auto targetStateIndex = state.stateEndTransition.transitionInfo.targetStateIndex;

			if (targetStateIndex != -1)
				nodeEditor->StateDefaultTransition(i, targetStateIndex);

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
		auto& asmData = *GetContext().gocAnimator->asmResourceManager->animatorResource->binaryData;

		for (auto l : GetActiveLayers()) {
			if (l.prevState && l.nextState && l.prevState != l.nextState) {
				short prevId = static_cast<short>(l.prevState->stateData - asmData.states);
				short nextId = static_cast<short>(l.nextState->stateData - asmData.states);
				
				RenderFlow(prevId, nextId);
			}
		}
	}

	void ASMEditor::RenderFlow(short prevStateId, short nextStateId)
	{
		auto& asmData = *GetContext().gocAnimator->asmResourceManager->animatorResource->binaryData;
		auto& state = asmData.states[prevStateId];

		if (state.stateEndTransition.transitionInfo.targetStateIndex == nextStateId) {
			nodeEditor->StateDefaultTransitionFlow(prevStateId, nextStateId);
			return;
		}

		for (unsigned short j = 0; j < state.eventCount; j++) {
			auto& event = asmData.events[state.eventOffset + j];

			if (event.transition.transitionInfo.targetStateIndex == nextStateId) {
				nodeEditor->StateEventTransitionFlow(prevStateId, nextStateId, j);
				return;
			}
		}

		if (state.transitionArrayIndex == -1 || !nodeEditor->IsStateSelected(prevStateId))
			nodeEditor->StateTransition(prevStateId, nextStateId);

		nodeEditor->StateTransitionFlow(prevStateId, nextStateId);
	}
}
