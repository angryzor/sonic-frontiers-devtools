#include "Timeline.h"
#include "Context.h"
#include "Behaviors.h"
#include "SurfRideElement.h"
#include "implot_internal.h"

namespace ui::operation_modes::modes::surfride_editor {
	using namespace ucsl::resources::swif::v6;

	void Timeline::RenderPanel()
	{
		SurfRide::Layer* focusedLayer{};

		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

		for (auto& element : selection) {
			SurfRide::Layer* newLayer{};
			if (element.type == SurfRideElement::Type::LAYER) {
				newLayer = element.layer;
			}
			else if (element.type == SurfRideElement::Type::CAST) {
				newLayer = element.cast->layer;
			}

			if (newLayer != nullptr) {
				if (focusedLayer != nullptr) {
					ImGui::Text("Multiple layers selected");
					return;
				}
				else
					focusedLayer = newLayer;
			}
		}

		if (focusedLayer == nullptr) {
			ImGui::Text("Select a layer in the left pane.");
			return;
		}

		if (ImGui::BeginChild("Animation list", ImVec2(100.0f, 0.0f))) {
			for (int i = 0; i < focusedLayer->layerData->animationCount; i++) {
				if (ImGui::Selectable(focusedLayer->layerData->animations[i].name, focusedLayer->currentAnimationIndex == i))
					focusedLayer->StartAnimation(i);
				if (focusedLayer->currentAnimationIndex == i)
					ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndChild();

		auto& animation = focusedLayer->layerData->animations[focusedLayer->currentAnimationIndex];

		ImGui::SameLine();
		if (ImGui::BeginChild("Timeline", ImVec2(0,0), 0, ImGuiWindowFlags_HorizontalScrollbar)) {
			ImGui::SliderFloat("Zoom level", &zoom, 0.1f, 10.0f);
			ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));
			ImPlot::PushStyleVar(ImPlotStyleVar_PlotBorderSize, 0);
			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
			if (ImGui::BeginTable("Timeline", 2, ImGuiTableFlags_BordersInnerH)) {
				ImGui::TableSetupColumn("Cast name", ImGuiTableColumnFlags_WidthFixed, 100.0f);
				ImGui::TableSetupColumn("Timeline", ImGuiTableColumnFlags_WidthFixed, animation.frameCount * zoom);
				ImGui::TableHeadersRow();

				for (size_t i = 0; i < animation.motionCount; i++) {
					ImGui::TableNextRow();
					RenderMotion(*focusedLayer, animation, animation.motions[i]);
				}

				ImGui::EndTable();
			}
			ImGui::PopStyleVar();
			ImPlot::PopStyleVar(2);
		}
		ImGui::EndChild();
	}

	PanelTraits Timeline::GetPanelTraits() const
	{
		return { "Timeline", ImVec2(0, ImGui::GetMainViewport()->WorkSize.y - 575), ImVec2(1200, 550), ImVec2(1, 0) };
	}

	void Timeline::RenderMotion(SurfRide::Layer& layer, SRS_ANIMATION& animation, SRS_MOTION& motion)
	{
		ImGui::TableNextColumn();

		SRS_CASTNODE* cast{};
		for (size_t i = 0; i < layer.layerData->castCount; i++)
			if (layer.layerData->casts[i].id == motion.castId)
				cast = &layer.layerData->casts[i];

		ImGui::Text("%s", cast == nullptr ? "MISSINGNO." : cast->name);
		ImGui::TableNextColumn();
		auto cellPos = ImGui::GetCursorPos();
		auto cellScreenPos = ImGui::GetCursorScreenPos();
		auto playHeadFrame = std::fminf(layer.currentFrame3, static_cast<float>(animation.frameCount));
		for (size_t i = 0; i < motion.trackCount; i++) {
			RenderTrack(layer, animation, motion, motion.tracks[i]);
		}

		auto sliceCount = animation.frameCount / 60;
		auto sliceRemainder = animation.frameCount % 60;

		for (size_t i = 0; i < sliceCount; i++)
			ImGui::GetCurrentWindow()->DrawList->AddRectFilled(ImVec2(cellScreenPos.x + i * 60 * zoom, cellScreenPos.y), ImVec2(cellScreenPos.x + (i * 60 + 30) * zoom, ImGui::GetCursorScreenPos().y), 0x20FFFFFF);
		if (sliceRemainder > 0)
			ImGui::GetCurrentWindow()->DrawList->AddRectFilled(ImVec2(cellScreenPos.x + sliceCount * 60 * zoom, cellScreenPos.y), ImVec2(cellScreenPos.x + (sliceCount * 60 + std::min(30u, sliceRemainder)) * zoom, ImGui::GetCursorScreenPos().y), 0x20FFFFFF);
		ImGui::GetCurrentWindow()->DrawList->AddLine(ImVec2(cellScreenPos.x + playHeadFrame * zoom, cellScreenPos.y), ImVec2(cellScreenPos.x + playHeadFrame * zoom, ImGui::GetCursorScreenPos().y), 0xFFFFFFFF);
	}

	void Timeline::RenderTrack(SurfRide::Layer& layer, SRS_ANIMATION& animation, SRS_MOTION& motion, SRS_TRACK& track)
	{
		auto width = (track.lastFrame - track.firstFrame) * zoom;
		auto posBefore = ImGui::GetCursorPos();
		ImGui::SetCursorPosX(posBefore.x + track.firstFrame * zoom);
		ImGui::PushID(&track);
		if (width == 0) {
			if (ImGui::BeginChild("TimelineItem", ImVec2(20, 20))) {
				ImGui::GetCurrentWindow()->DrawList->AddCircleFilled(ImGui::GetCursorScreenPos() + ImVec2(0, 10), 5, 0xFF5A6AED);
				ImGui::GetCurrentWindow()->DrawList->AddCircle(ImGui::GetCursorScreenPos() + ImVec2(0, 10), 5, 0xFF000000);
			}
			ImGui::EndChild();
			//if (ImGui::IsItemClicked()) {
			//	ImGui::OpenPopup("Editor");
			//}
			//assert(track.keyCount == 1);
			//if (ImGui::BeginPopup("Editor")) {
			//	RenderKeyFrameValueEditor(track, track.pKeyFrame[0]);
			//	ImGui::EndPopup();
			//}
			ImGui::SetCursorPos(posBefore + ImVec2(0, 20));
		}
		else {
			auto height = 80.0f;
			if (ImGui::BeginChild("TimelineItem", ImVec2(width, height + ImGui::GetTextLineHeightWithSpacing()))) {
				ImGui::GetCurrentWindow()->DrawList->AddRectFilled(ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + ImVec2(width, ImGui::GetTextLineHeightWithSpacing()), 0xFF5A6AED);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				ImGui::Text("%s", TrackName(track));
				ImGui::PopStyleColor();
				ImGui::SetItemTooltip("%s", TrackName(track));
				if (ImPlot::BeginPlot("##Track", ImVec2(width, height), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
					ImPlot::SetupAxis(ImAxis_X1, "Time", ImPlotAxisFlags_NoDecorations);
					ImPlot::SetupAxis(ImAxis_Y1, "Value", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
					ImPlot::SetupAxisLimits(ImAxis_X1, track.firstFrame, track.lastFrame, ImPlotCond_Always);
					RenderPlotLines(track);
					ImPlot::EndPlot();
				}

				// Have to create a new child because ImPlot creates some and we need this to enforce draw order
				ImGui::SetCursorPos(ImVec2(0, 0));
				if (ImGui::BeginChild("overlays", ImVec2(width, height + ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_None, ImGuiWindowFlags_NoInputs)) {
					ImGui::GetCurrentWindow()->DrawList->AddRect(ImGui::GetCursorScreenPos(), ImGui::GetCursorScreenPos() + ImVec2(width, height + ImGui::GetTextLineHeightWithSpacing()), 0xFF000000);
					ImGui::GetCurrentWindow()->DrawList->AddLine(ImGui::GetCursorScreenPos() + ImVec2(0, ImGui::GetTextLineHeightWithSpacing()), ImGui::GetCursorScreenPos() + ImVec2(width, ImGui::GetTextLineHeightWithSpacing()), 0xFF000000);
				}
				ImGui::EndChild();
			}
			ImGui::EndChild();
			ImGui::SetCursorPos(posBefore + ImVec2(0, height + ImGui::GetTextLineHeightWithSpacing()));
		}
		ImGui::PopID();
	}




	double GetR(const Color& color) { return color.r; }
	double GetG(const Color& color) { return color.g; }
	double GetB(const Color& color) { return color.b; }
	double GetA(const Color& color) { return color.a; }
	void SetR(Color& color, double y) { color.r = static_cast<uint8_t>(std::round(y)); }
	void SetG(Color& color, double y) { color.g = static_cast<uint8_t>(std::round(y)); }
	void SetB(Color& color, double y) { color.b = static_cast<uint8_t>(std::round(y)); }
	void SetA(Color& color, double y) { color.a = static_cast<uint8_t>(std::round(y)); }

	void Timeline::RenderPlotLines(SRS_TRACK& track)
	{
		if (track.keyCount == 0)
			return;

		switch (track.GetDataType()) {
		case ETrackDataType::FLOAT:
			switch (track.trackType) {
			case ECurveType::MaterialColorR:
			case ECurveType::MaterialColorG:
			case ECurveType::MaterialColorB:
			case ECurveType::MaterialColorA:
			case ECurveType::VertexColorTopLeftR:
			case ECurveType::VertexColorTopLeftG:
			case ECurveType::VertexColorTopLeftB:
			case ECurveType::VertexColorTopLeftA:
			case ECurveType::VertexColorTopRightR:
			case ECurveType::VertexColorTopRightG:
			case ECurveType::VertexColorTopRightB:
			case ECurveType::VertexColorTopRightA:
			case ECurveType::VertexColorBottomLeftR:
			case ECurveType::VertexColorBottomLeftG:
			case ECurveType::VertexColorBottomLeftB:
			case ECurveType::VertexColorBottomLeftA:
			case ECurveType::VertexColorBottomRightR:
			case ECurveType::VertexColorBottomRightG:
			case ECurveType::VertexColorBottomRightB:
			case ECurveType::VertexColorBottomRightA:
			case ECurveType::IlluminationColorR:
			case ECurveType::IlluminationColorG:
			case ECurveType::IlluminationColorB:
			case ECurveType::IlluminationColorA:
				SetupYAxis(0, 1);
				RenderDragPoints<float>(track, 0.0f, 1.0f);
				break;
			default:
				SetupFloatingYAxis<float>(track);
				RenderDragPoints<float>(track);
				break;
			}

			RenderPlotLine<float>(track);
			break;
		case ETrackDataType::BOOL:
			SetupYAxis(false, true);
			RenderDragPoints<bool>(track, false, true);
			RenderPlotLine<bool>(track);
			break;
		case ETrackDataType::INT:
			SetupFloatingYAxis<int>(track);
			RenderDragPoints<int>(track);
			RenderPlotLine<int>(track);
			break;
		case ETrackDataType::INDEX:
			SetupFloatingYAxis<int>(track);
			RenderDragPoints<int>(track);
			RenderPlotLine<int>(track);
			break;
		case ETrackDataType::COLOR:
			SetupYAxis(0, 255);

			RenderDragPoints<Color, GetR, SetR>(track, 0, 255, "R", ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			RenderDragPoints<Color, GetG, SetG>(track, 0, 255, "G", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
			RenderDragPoints<Color, GetB, SetB>(track, 0, 255, "B", ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
			RenderDragPoints<Color, GetA, SetA>(track, 0, 255, "A", ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

			RenderPlotLine<Color, GetR>(track, "R", ImVec4(1.0f, 0.0f, 0.0f, 1.0f), false);
			RenderPlotLine<Color, GetG>(track, "G", ImVec4(0.0f, 1.0f, 0.0f, 1.0f), false);
			RenderPlotLine<Color, GetB>(track, "B", ImVec4(0.0f, 0.0f, 1.0f, 1.0f), false);
			RenderPlotLine<Color, GetA>(track, "A", ImVec4(0.0f, 0.0f, 0.0f, 1.0f), false);
			break;
		}
	}

	const char* Timeline::TrackName(SRS_TRACK& track)
	{
		switch (track.trackType) {
		case ECurveType::TranslationX: return "Translation X";
		case ECurveType::TranslationY: return "Translation Y";
		case ECurveType::TranslationZ: return "Translation Z";
		case ECurveType::RotationX: return "Rotation X";
		case ECurveType::RotationY: return "Rotation Y";
		case ECurveType::RotationZ: return "Rotation Z";
		case ECurveType::ScaleX: return "Scale X";
		case ECurveType::ScaleY: return "Scale Y";
		case ECurveType::ScaleZ: return "Scale Z";
		case ECurveType::MaterialColorR: return "Material Color - Red";
		case ECurveType::MaterialColorG: return "Material Color - Green";
		case ECurveType::MaterialColorB: return "Material Color - Blue";
		case ECurveType::MaterialColorA: return "Material Color - Alpha";
		case ECurveType::VertexColorTopLeftR: return "Vertex Color Top Left - Red";
		case ECurveType::VertexColorTopLeftG: return "Vertex Color Top Left - Green";
		case ECurveType::VertexColorTopLeftB: return "Vertex Color Top Left - Blue";
		case ECurveType::VertexColorTopLeftA: return "Vertex Color Top Left - Alpha";
		case ECurveType::VertexColorTopRightR: return "Vertex Color Top Right - Red";
		case ECurveType::VertexColorTopRightG: return "Vertex Color Top Right - Green";
		case ECurveType::VertexColorTopRightB: return "Vertex Color Top Right - Blue";
		case ECurveType::VertexColorTopRightA: return "Vertex Color Top Right - Alpha";
		case ECurveType::VertexColorBottomLeftR: return "Vertex Color Bottom Left - Red";
		case ECurveType::VertexColorBottomLeftG: return "Vertex Color Bottom Left - Green";
		case ECurveType::VertexColorBottomLeftB: return "Vertex Color Bottom Left - Blue";
		case ECurveType::VertexColorBottomLeftA: return "Vertex Color Bottom Left - Alpha";
		case ECurveType::VertexColorBottomRightR: return "Vertex Color Bottom Right - Red";
		case ECurveType::VertexColorBottomRightG: return "Vertex Color Bottom Right - Green";
		case ECurveType::VertexColorBottomRightB: return "Vertex Color Bottom Right - Blue";
		case ECurveType::VertexColorBottomRightA: return "Vertex Color Bottom Right - Alpha";
		case ECurveType::IlluminationColorR: return "Illumination Color - Red";
		case ECurveType::IlluminationColorG: return "Illumination Color - Green";
		case ECurveType::IlluminationColorB: return "Illumination Color - Blue";
		case ECurveType::IlluminationColorA: return "Illumination Color - Alpha";
		case ECurveType::Display: return "Display";
		case ECurveType::Width: return "Width";
		case ECurveType::Height: return "Height";
		case ECurveType::CropIndex0: return "Crop Index 0";
		case ECurveType::CropIndex1: return "Crop Index 1";
		case ECurveType::VertexColorTopLeft: return "Vertex Color Top Left";
		case ECurveType::VertexColorTopRight: return "Vertex Color Top Right";
		case ECurveType::VertexColorBottomLeft: return "Vertex Color Bottom Left";
		case ECurveType::VertexColorBottomRight: return "Vertex Color Bottom Right";
		case ECurveType::MaterialColor: return "Material Color";
		case ECurveType::IlluminationColor: return "Illumination Color";
		case ECurveType::Unknown: return "Unknown";
		default: return "Unknown";
		}
	}

	const char* Timeline::interpolationTypes[3] = { "Constant", "Linear", "Hermite" };
}
