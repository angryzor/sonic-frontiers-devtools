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

		if (!autoLayouted) {
			for (unsigned short i = 0; i < asmData.stateCount; i++) {
				auto dims = ImGui::CalcTextSize(asmData.states[i].name);
				ogdf::node n = autoLayoutGraph.newNode();
				autoLayoutGraphAttributes.height(n) = dims.x * 1.4f + 20.0f;
				autoLayoutGraphAttributes.width(n) = dims.y * 1.4f + 10.0f;
				autoLayout.push_back(n);
			}

			for (unsigned short i = 0; i < asmData.stateCount; i++) {
				auto& state = asmData.states[i];
				auto targetStateIndex = state.stateEndTransition.transitionInfo.targetStateIndex;

				if (targetStateIndex != -1)
					autoLayoutGraph.newEdge(autoLayout[i], autoLayout[targetStateIndex]);

				for (unsigned short j = 0; j < state.eventCount; j++) {
					auto& event = asmData.events[state.eventOffset + j];

					autoLayoutGraph.newEdge(autoLayout[i], autoLayout[event.transition.transitionInfo.targetStateIndex]);
				}

				//if (state.transitionArrayIndex != -1) {
				//	auto& transitionArray = asmData.transitionArrays[state.transitionArrayIndex];
				//	auto* transitions = &asmData.transitions[transitionArray.offset];

				//	for (int j = 0; j < transitionArray.size; j++)
				//		autoLayoutGraph.newEdge(autoLayout[i], autoLayout[transitions[j].transitionInfo.targetStateIndex]);
				//}
			}

			ogdf::SugiyamaLayout sl{};
			sl.setRanking(new ogdf::OptimalRanking{});
			sl.setCrossMin(new ogdf::MedianHeuristic{});

			//auto* fhl = new ogdf::FastHierarchyLayout{};
			//fhl->layerDistance(300.0);
			//fhl->nodeDistance(50.0);
			//sl.setLayout(fhl);

			auto* ohl = new ogdf::OptimalHierarchyLayout{};
			//ohl->layerDistance(300.0);
			ohl->nodeDistance(50.0);
			ohl->weightBalancing(0.8);
			sl.setLayout(ohl);
			sl.call(autoLayoutGraphAttributes);
		}

		ImPlot::PushColormap(ImPlotColormap_Cool);
		NodeEd::SetCurrentEditor(edContext);
		NodeEd::PushStyleVar(NodeEd::StyleVar_FlowDuration, 0.25f);
		NodeEd::PushStyleVar(NodeEd::StyleVar_FlowMarkerDistance, 45.0f);
		NodeEd::PushStyleVar(NodeEd::StyleVar_FlowSpeed, 300.0f);
		NodeEd::Begin("Editor");

		//csl::ut::MoveArray<std::optional<short>> activeStates{ hh::fnd::MemoryRouter::GetTempAllocator() };
		//csl::ut::MoveArray<std::optional<std::pair<short, short>>> activeTransitions{ hh::fnd::MemoryRouter::GetTempAllocator() };

		//for (auto l : context.gocAnimator->animationStateMachine->layers) {
		//	if (l.layerState == nullptr)
		//		continue;

		//	auto* prevAnimState = l.layerState->GetPreviousAnimationState();
		//	auto* nextAnimState = l.layerState->GetNextAnimationState();

		//	activeStates.push_back(nextAnimState ? std::make_optional(static_cast<short>(nextAnimState->stateData - asmData.states)) : std::nullopt);
		//	activeTransitions.push_back(prevAnimState != nullptr && prevAnimState != nextAnimState ? std::make_optional(std::make_pair(prevAnimState->stateData - asmData.states, nextAnimState->stateData - asmData.states)) : std::nullopt);
		//}


		for (unsigned short i = 0; i < asmData.stateCount; i++) {
			auto& state = asmData.states[i];
			auto autoLayoutNode = autoLayout[i];

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
			if (!autoLayouted)
				NodeEd::SetNodePosition(GetStateId(i), { static_cast<float>(autoLayoutGraphAttributes.y(autoLayoutNode)), static_cast<float>(autoLayoutGraphAttributes.x(autoLayoutNode)) });
			NodeEd::BeginNode(GetStateId(i));

			ImGui::PushID(i);

			ImGui::BeginGroup();
			NodeEd::PushStyleVar(NodeEd::StyleVar_PinArrowSize, 10.0f);
			NodeEd::PushStyleVar(NodeEd::StyleVar_PinArrowWidth, 10.0f);
			NodeEd::BeginPin(GetInputPinId(i), NodeEd::PinKind::Input);
			ImGui::Dummy({ 10.0f, 10.0f });
			//ImGui::Text("In");
			NodeEd::EndPin();
			NodeEd::PopStyleVar(2);
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			ImGui::Text("%s", state.name);
			ImGui::EndGroup();

			ImGui::SameLine();

			ImGui::BeginGroup();
			NodeEd::BeginPin(GetOutputPinId(i), NodeEd::PinKind::Output);
			ImGui::Dummy({ 10.0f, 10.0f });
			//ImGui::Text("Out");
			NodeEd::EndPin();
			ImGui::EndGroup();

			ImGui::PopID();

			NodeEd::EndNode();
			NodeEd::PopStyleColor();
		}

		for (unsigned short i = 0; i < asmData.stateCount; i++) {
			auto& state = asmData.states[i];
			auto targetStateIndex = state.stateEndTransition.transitionInfo.targetStateIndex;
			
			if (targetStateIndex != -1) {
				NodeEd::Link(GetLinkId(i, targetStateIndex), GetOutputPinId(i), GetInputPinId(targetStateIndex), { 0.6f, 0.6f, 0.6f, 1.0f });
			}


			for (unsigned short j = 0; j < state.eventCount; j++) {
				auto& event = asmData.events[state.eventOffset + j];

				NodeEd::Link(GetLinkId(i, event.transition.transitionInfo.targetStateIndex), GetOutputPinId(i), GetInputPinId(event.transition.transitionInfo.targetStateIndex), { 0.0f, 0.4f, 0.8f, 1.0f });
			}

			if (state.transitionArrayIndex != -1) {
				if (NodeEd::IsNodeSelected(GetStateId(i))) {
					auto& transitionArray = asmData.transitionArrays[state.transitionArrayIndex];
					auto* transitions = &asmData.transitions[transitionArray.offset];

					for (int j = 0; j < transitionArray.size; j++)
						NodeEd::Link(GetLinkId(i, transitions[j].transitionInfo.targetStateIndex), GetOutputPinId(i), GetInputPinId(transitions[j].transitionInfo.targetStateIndex), { 0.6f, 0.6f, 0.0f, 1.0f });
				}
				else {
					for (auto l : context.gocAnimator->animationStateMachine->layers) {
						if (l.layerState == nullptr)
							continue;

						auto* prevAnimState = l.layerState->GetPreviousAnimationState();
						auto* nextAnimState = l.layerState->GetNextAnimationState();

						if (prevAnimState && nextAnimState && prevAnimState != nextAnimState) {
							short prevId = prevAnimState->stateData - asmData.states;
							short nextId = nextAnimState->stateData - asmData.states;

							if (prevId == i)
								NodeEd::Link(GetLinkId(prevId, nextId), GetOutputPinId(prevId), GetInputPinId(nextId));
						}
					}
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

				auto layerColor = ImPlot::GetColormapColor(l.layerId);

				NodeEd::Flow(GetLinkId(prevId, nextId));
			}
		}

		NodeEd::End();
		NodeEd::PopStyleVar(3);
		NodeEd::SetCurrentEditor(nullptr);
		ImPlot::PopColormap();

		autoLayouted = true;
	}

	ax::NodeEditor::NodeId ASMEditor::GetStateId(short stateId)
	{
		return stateId;
	}

	ax::NodeEditor::PinId ASMEditor::GetInputPinId(short stateId)
	{
		return (stateId << 1) | 0;
	}

	ax::NodeEditor::PinId ASMEditor::GetOutputPinId(short stateId)
	{
		return (stateId << 1) | 1;
	}

	ax::NodeEditor::LinkId ASMEditor::GetLinkId(short fromStateId, short toStateId)
	{
		return (fromStateId << 16) | toStateId;
	}
}
