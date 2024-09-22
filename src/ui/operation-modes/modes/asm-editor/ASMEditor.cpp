#include "ASMEditor.h"
#include <ui/common/editors/Basic.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/FastHierarchyLayout.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <optional>

namespace ui::operation_modes::modes::asm_editor
{
	using namespace hh::anim;
	namespace NodeEd = ax::NodeEditor;

	const char* stateTypeNames[]{ "Clip", "Blend tree", "None" };

	ASMEditor::ASMEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host) : OperationMode{ allocator, host }
	{
		ax::NodeEditor::Config config{};
		edContext = NodeEd::CreateEditor(&config);
	}

	ASMEditor::~ASMEditor()
	{
		NodeEd::DestroyEditor(edContext);
	}

	void ASMEditor::RenderScene()
	{
		auto& context = GetContext();

		//if (context.gocAnimator == nullptr || context.focusedLayer == -1)
		//	return;

		auto& asmData = *context.gocAnimator->asmResourceManager->animatorResource->binaryData;

		ImPlot::PushColormap(ImPlotColormap_Deep);
		NodeEd::SetCurrentEditor(edContext);
		NodeEd::PushStyleVar(NodeEd::StyleVar_FlowDuration, 0.25f);
		NodeEd::PushStyleVar(NodeEd::StyleVar_FlowMarkerDistance, 45.0f);
		NodeEd::PushStyleVar(NodeEd::StyleVar_FlowSpeed, 300.0f);
		NodeEd::Begin("Editor");
		autoLayout.Begin();

		for (unsigned short i = 0; i < asmData.stateCount; i++) {
			auto& state = asmData.states[i];

			auto color = NodeEd::GetStyle().Colors[NodeEd::StyleColor_NodeBorder];
			AnimationStateMachine::LayerInfo* layer = nullptr;

			for (auto l : context.gocAnimator->animationStateMachine->layers) {
				if (l.layerState == nullptr)
					continue;

				auto* prevAnimState = l.layerState->GetPreviousAnimationState();
				auto* nextAnimState = l.layerState->GetNextAnimationState();

				auto layerColor = ImPlot::GetColormapColor(l.layerId);

				if (nextAnimState && nextAnimState->stateData == &state) {
					color = layerColor;
					break;
				}
				else if (prevAnimState && prevAnimState->stateData == &state) {
					color = { layerColor.x * 0.7f, layerColor.y * 0.7f, layerColor.z * 0.7f, 1.0f };
					break;
				}
			}

			NodeEd::PushStyleColor(NodeEd::StyleColor_NodeBorder, color);
			NodeEd::BeginNode(GetNodeId(i));

			ImGui::PushID(i);

			ImGui::BeginGroup();
			RenderInputPin(i);
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			auto textSize = ImGui::CalcTextSize(state.name).x + 10.0f;
			bool found = false;
			for (auto l : context.gocAnimator->animationStateMachine->layers) {
				if (l.layerState == nullptr)
					continue;

				auto* prevAnimState = l.layerState->GetPreviousAnimationState();
				auto* nextAnimState = l.layerState->GetNextAnimationState();

				auto layerColor = ImPlot::GetColormapColor(l.layerId);

				if (nextAnimState && nextAnimState->stateData == &state) {
					ImGui::ProgressBar(nextAnimState->implementation.currentTime / nextAnimState->implementation.duration, { textSize, 0.0f }, state.name);
					found = true;
					break;
				}
				else if (prevAnimState && prevAnimState->stateData == &state) {
					ImGui::ProgressBar(prevAnimState->implementation.currentTime / prevAnimState->implementation.duration, { textSize, 0.0f }, state.name);
					found = true;
					break;
				}
			}
			if (!found)
				ImGui::ProgressBar(0.0f, { textSize, 0.0f }, state.name);

			Editor("Duration", state.speed);
			if (state.type == StateType::CLIP) {
				auto& clipData = asmData.clips[state.rootBlendNodeOrClipIndex];
				ImGui::Text("Resanem: %s", clipData.name);
				//Viewer("Animation resource", clipData.name);
				Editor("Animation start", clipData.animationSettings.start);
				Editor("Animation end", clipData.animationSettings.end);
				Editor("Animation speed", clipData.animationSettings.speed);
				Editor("Looping", clipData.animationSettings.loops);
			}

			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();

			float maxTextWidth = 0.0f;
			for (unsigned short j = 0; j < state.eventCount; j++) {
				auto& event = asmData.events[state.eventOffset + j];
				maxTextWidth = std::fmaxf(maxTextWidth, ImGui::CalcTextSize(event.name).x);
			}

			RenderDefaultTransitionPin(i, maxTextWidth);
			RenderTransitionPin(i, maxTextWidth);

			for (unsigned short j = 0; j < state.eventCount; j++)
				RenderEventPin(i, j, asmData.events[state.eventOffset + j], maxTextWidth);

			ImGui::EndGroup();

			ImGui::PopID();

			NodeEd::EndNode();
			autoLayout.AddNode(GetNodeId(i));
			NodeEd::PopStyleColor();
		}

		for (unsigned short i = 0; i < asmData.stateCount; i++) {
			auto& state = asmData.states[i];
			auto targetStateIndex = state.stateEndTransition.transitionInfo.targetStateIndex;

			if (targetStateIndex != -1) {
				RenderLink(GetDefaultTransitionPinId(i), targetStateIndex, {1.0f, 1.0f, 1.0f, 1.0f});
				autoLayout.AddLink(GetNodeId(i), GetNodeId(targetStateIndex));
			}


			for (unsigned short j = 0; j < state.eventCount; j++) {
				auto& event = asmData.events[state.eventOffset + j];

				RenderLink(GetEventPinId(i, j), event.transition.transitionInfo.targetStateIndex, {0.965f, 0.243f, 0.235f, 1.0f});
				autoLayout.AddLink(GetNodeId(i), GetNodeId(event.transition.transitionInfo.targetStateIndex));
			}

			if (state.transitionArrayIndex != -1) {
				if (NodeEd::IsNodeSelected(GetNodeId(i))) {
					auto& transitionArray = asmData.transitionArrays[state.transitionArrayIndex];
					auto* transitions = &asmData.transitions[transitionArray.offset];

					for (int j = 0; j < transitionArray.size; j++)
						RenderLink(GetTransitionPinId(i), transitions[j].transitionInfo.targetStateIndex, { 0.0f, 0.8f, 0.8f, 1.0f });
				}
			}
		}

		for (auto l : context.gocAnimator->animationStateMachine->layers) {
			if (l.layerState == nullptr)
				continue;

			auto* prevAnimState = l.layerState->GetPreviousAnimationState();
			auto* nextAnimState = l.layerState->GetNextAnimationState();

			if (prevAnimState && nextAnimState && prevAnimState != nextAnimState) {
				short prevId = prevAnimState->stateData - asmData.states;
				short nextId = nextAnimState->stateData - asmData.states;
				auto outputPin = ResolveOutputPin(prevId, nextId);

				NodeEd::Flow(GetLinkId(outputPin, nextId));
			}
		}

		autoLayout.End();

		NodeEd::End();
		NodeEd::PopStyleVar(3);
		NodeEd::SetCurrentEditor(nullptr);
		ImPlot::PopColormap();
	}

	ax::NodeEditor::NodeId ASMEditor::GetNodeId(short stateId)
	{
		return stateId;
	}

	ax::NodeEditor::PinId ASMEditor::GetPinId(short stateId, PinType type, unsigned short idx)
	{
		unsigned long long lStateId = stateId;
		unsigned long long lType = static_cast<unsigned long long>(type);
		unsigned long long lIdx = idx;

		return (lStateId << 16) | (lType << 14) | lIdx;
	}

	ax::NodeEditor::PinId ASMEditor::GetInputPinId(short stateId)
	{
		return GetPinId(stateId, PinType::INPUT, 0);
	}

	ax::NodeEditor::PinId ASMEditor::GetDefaultTransitionPinId(short stateId)
	{
		return GetPinId(stateId, PinType::DEFAULT_TRANSITION, 0);
	}

	ax::NodeEditor::PinId ASMEditor::GetTransitionPinId(short stateId)
	{
		return GetPinId(stateId, PinType::TRANSITION, 0);
	}

	ax::NodeEditor::PinId ASMEditor::GetEventPinId(short stateId, unsigned short idx)
	{
		return GetPinId(stateId, PinType::EVENT, idx);
	}

	ax::NodeEditor::LinkId ASMEditor::GetLinkId(ax::NodeEditor::PinId fromPinId, short toState)
	{
		unsigned long long lFromPinId = fromPinId.Get();
		unsigned long long lToPinId = GetInputPinId(toState).Get();

		return (lFromPinId << 32) | lToPinId;
	}

	ax::NodeEditor::PinId ASMEditor::ResolveOutputPin(short prevStateId, short nextStateId)
	{
		auto& asmData = *GetContext().gocAnimator->asmResourceManager->animatorResource->binaryData;
		auto& state = asmData.states[prevStateId];

		if (state.stateEndTransition.transitionInfo.targetStateIndex == nextStateId)
			return GetDefaultTransitionPinId(prevStateId);
		
		for (unsigned short j = 0; j < state.eventCount; j++) {
			auto& event = asmData.events[state.eventOffset + j];

			if (event.transition.transitionInfo.targetStateIndex == nextStateId)
				return GetEventPinId(prevStateId, j);
		}

		if (state.transitionArrayIndex == -1 || !NodeEd::IsNodeSelected(GetNodeId(prevStateId)))
			RenderLink(GetTransitionPinId(prevStateId), nextStateId, { 0.0f, 0.8f, 0.8f, 1.0f });

		return GetTransitionPinId(prevStateId);
	}

	void ASMEditor::RenderInputPin(short stateId)
	{
		NodeEd::PushStyleVar(NodeEd::StyleVar_PinArrowSize, 10.0f);
		NodeEd::PushStyleVar(NodeEd::StyleVar_PinArrowWidth, 10.0f);
		NodeEd::BeginPin(GetInputPinId(stateId), NodeEd::PinKind::Input);
		ImGui::Dummy({ 10.0f, 10.0f });
		auto itemRectMin = ImGui::GetItemRectMin();
		ImVec2 arrowHead[] = { { itemRectMin.x, itemRectMin.y }, { itemRectMin.x + 5.0f, itemRectMin.y }, { itemRectMin.x + 10.0f, itemRectMin.y + 5.0f }, { itemRectMin.x + 5.0f, itemRectMin.y + 10.0f }, { itemRectMin.x, itemRectMin.y + 10.0f } };
		ImGui::GetWindowDrawList()->AddConvexPolyFilled(arrowHead, 5, ImGui::GetColorU32({ 1.0f, 1.0f, 1.0f, 1.0f }));
		NodeEd::EndPin();
		NodeEd::PopStyleVar(2);
	}

	void ASMEditor::RenderDefaultTransitionPin(short stateId, float textWidth)
	{
		NodeEd::BeginPin(GetDefaultTransitionPinId(stateId), NodeEd::PinKind::Output);
		ImGui::Dummy({ textWidth, 0.0f });
		ImGui::SameLine();
		ImGui::Dummy({ 10.0f, 10.0f });
		auto itemRectMin = ImGui::GetItemRectMin();
		ImVec2 arrowHead[] = { { itemRectMin.x, itemRectMin.y }, { itemRectMin.x + 5.0f, itemRectMin.y }, { itemRectMin.x + 10.0f, itemRectMin.y + 5.0f }, { itemRectMin.x + 5.0f, itemRectMin.y + 10.0f }, { itemRectMin.x, itemRectMin.y + 10.0f } };
		ImGui::GetWindowDrawList()->AddConvexPolyFilled(arrowHead, 5, ImGui::GetColorU32({ 1.0f, 1.0f, 1.0f, 1.0f }));
		NodeEd::EndPin();
	}

	void ASMEditor::RenderTransitionPin(short stateId, float textWidth)
	{
		NodeEd::BeginPin(GetTransitionPinId(stateId), NodeEd::PinKind::Output);
		ImGui::Dummy({ textWidth, 0.0f });
		ImGui::SameLine();
		ImGui::Dummy({ 10.0f, 10.0f });
		auto itemRectMin = ImGui::GetItemRectMin();
		ImVec2 arrowHead[] = { { itemRectMin.x, itemRectMin.y }, { itemRectMin.x + 5.0f, itemRectMin.y }, { itemRectMin.x + 10.0f, itemRectMin.y + 5.0f }, { itemRectMin.x + 5.0f, itemRectMin.y + 10.0f }, { itemRectMin.x, itemRectMin.y + 10.0f } };
		ImGui::GetWindowDrawList()->AddConvexPolyFilled(arrowHead, 5, ImGui::GetColorU32({ 0.0f, 0.8f, 0.8f, 1.0f }));
		NodeEd::EndPin();
	}

	void ASMEditor::RenderEventPin(short stateId, short eventId, EventData& event, float textWidth)
	{
		NodeEd::BeginPin(GetEventPinId(stateId, eventId), NodeEd::PinKind::Output);
		ImGui::Dummy({ textWidth - ImGui::CalcTextSize(event.name).x - ImGui::GetStyle().ItemSpacing.x, 0.0f});
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, { 0.965f, 0.243f, 0.235f, 1.0f });
		ImGui::Text("%s", event.name);
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::Dummy({ 10.0f, 10.0f });
		auto itemRectMin = ImGui::GetItemRectMin();
		auto itemRectMax = ImGui::GetItemRectMax();
		ImGui::GetWindowDrawList()->AddRectFilled(itemRectMin, itemRectMax, ImGui::GetColorU32({ 0.965f, 0.243f, 0.235f, 1.0f }));
		NodeEd::EndPin();
	}

	void ASMEditor::RenderLink(ax::NodeEditor::PinId sourcePin, short targetStateId, const ImVec4& color)
	{
		NodeEd::Link(GetLinkId(sourcePin, targetStateId), sourcePin, GetInputPinId(targetStateId), color);
	}
}
