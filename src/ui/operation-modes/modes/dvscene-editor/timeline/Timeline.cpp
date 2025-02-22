#include "Timeline.h"
#include "../Behaviors.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <imtimeline.h>
#include <ui/GlobalSettings.h>
#include <algorithm>
#include <numeric>
#include "Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
	Timeline::Timeline(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode) : Panel{ allocator, operationMode }, timelineCtx{ ImTimeline::CreateContext() } { }

	Timeline::~Timeline() {
		ImTimeline::DestroyContext(timelineCtx);
	}

	void Timeline::RenderPanel()
	{
		auto& context = GetContext();

		if (context.goDVSC == nullptr){
            ImGui::Text("No DvSceneControl selected");
			return;
		}
        
		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		auto& selected = selectionBehavior->GetSelection();

        if (ImGui::BeginChild("Timeline", ImVec2(0,0), 0, ImGuiWindowFlags_HorizontalScrollbar)) {
			ImGui::Checkbox("Looping", &context.goDVSC->timeline->looping);
			auto playHeadFrame = std::fminf(context.goDVSC->timeline->currentFrame0/100, static_cast<float>(context.goDVSC->timeline->frameEnd/100));
			bool currentTimeChanged{};

			ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));
			ImPlot::PushStyleVar(ImPlotStyleVar_PlotBorderSize, 0);
			ImTimeline::Begin(timelineCtx);
			ImGui::SameLine();
			if (ImTimeline::BeginTimeline("Timeline", &playHeadFrame, static_cast<float>(context.goDVSC->timeline->frameEnd/100), 60.0, &context.goDVSC->play, &currentTimeChanged)) {
				if (selected.size() > 0) {
					hh::dv::DvNodeBase* node = selected[0].node;
					int type = static_cast<int>(node->nodeType);
					NodeTimelineFuncType render = GetNodeTimelineRender(type);
					if(render)
						render(this, node);
				}

				ImTimeline::EndTimeline();
			}
			ImTimeline::End();
			ImPlot::PopStyleVar(2);
			context.goDVSC->timeline->currentFrame0 = static_cast<int>(playHeadFrame * 100);
			context.goDVSC->timeline->currentFrame1 = static_cast<int>(playHeadFrame * 100);
		}
		ImGui::EndChild();
	}

	PanelTraits Timeline::GetPanelTraits() const
	{
		return { "Timeline", ImVec2(250, 500), ImVec2(500, 250) };
	}

	void Timeline::RenderTimeline(int& start, int& end, float* curve, int size, bool axisLimit, float maxValue) {
		if (ImTimeline::BeginTrack("")) {
			auto length = end / 100 - start / 100;

			if (length == 0) {
				float time{ 0.0f };
				ImTimeline::Event("", &time);
			}
			else {
				float startTime = static_cast<float>(start / 100);
				float endTime = static_cast<float>(end / 100);
				bool startTimeChanged{};
				bool endTimeChanged{};
				bool moved{};

				ImVec4 color = ImVec4(0.31f, 0.69f, 0.776f, 1.0f);
				ImPlotPoint clickpos{};

				if (ImTimeline::BeginClip("", &startTime, &endTime, 80.0f, &startTimeChanged, &endTimeChanged, &moved)) {
					if (curve && ImPlot::BeginPlot("##Track", ImTimeline::GetClipSize(), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
						if (ImGui::BeginPopupContextItem("Controls")) {
							Editor("Falloff", GetContext().timelineFalloff);
							ImGui::EndPopup();
						}

						ImPlot::SetupAxis(ImAxis_X1, "Time", ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_Lock | ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax);
						ImPlot::SetupAxis(ImAxis_Y1, "Value", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
						ImPlot::SetupAxisLimits(ImAxis_X1, 0, size, ImPlotCond_Always);
						if(axisLimit)
							ImPlot::SetupAxisLimits(ImAxis_Y1, 0, maxValue, ImPlotCond_Always);

						float* time = new float[size];
						std::iota(time, time + size, 0.0f);
						ImPlot::SetNextFillStyle(color, 0.3f);
						ImPlot::PlotLine<float>("X", time, curve, size, ImPlotLineFlags_Shaded);
						delete[] time;
						for (auto x = 0; x < size; x++) {
							double frame = x;
							double value = curve[x];
							bool clicked;
							bool hovered;
							bool held;

							if (ImPlot::DragPoint(x, &frame, &value, color, 4.0f, 0, &clicked, &hovered, &held)) {
								if (axisLimit)
									value = std::clamp(value, 0.0, static_cast<double>(maxValue));

								double sigma = GetContext().timelineFalloff;
								double delta = value - curve[x];

								for (int i = 0; i < size; i++) {
									double dist = i - x;
									double weight = std::exp(-(dist * dist) / (sigma * sigma));
									curve[i] += weight*delta;
									if (axisLimit)
										curve[i] = std::clamp(static_cast<double>(curve[i]), 0.0, static_cast<double>(maxValue));
								}
							}

							if (hovered || held)
								ImGui::SetTooltip("Frame: %d\nValue: %f", x, value);
						}

						clickpos = ImPlot::GetPlotMousePos(ImAxis_X1, ImAxis_Y1);

						ImPlot::EndPlot();
					}
					ImTimeline::EndClip();
				}

				if (startTimeChanged)
					start = static_cast<int>(startTime * 100);

				if (endTimeChanged)
					end = static_cast<int>(endTime * 100);
			}
			ImTimeline::EndTrack();
		}
	}
}
