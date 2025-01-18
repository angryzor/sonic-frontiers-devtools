#pragma once
#include <ui/operation-modes/Panel.h>
#include <ui/common/inputs/Basic.h>
#include <imtimeline.h>
#include "Context.h"

namespace ui::operation_modes::modes::surfride_editor {
	class Timeline : public Panel<Context> {
		ImTimeline::ImTimelineContext* timelineCtx;

	public:
		Timeline(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode);
		virtual ~Timeline();

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;
		void RenderMotion(SurfRide::Layer& layer, ucsl::resources::swif::v6::SRS_ANIMATION& animation, ucsl::resources::swif::v6::SRS_MOTION& motion);
		void RenderTrack(SurfRide::Layer& layer, ucsl::resources::swif::v6::SRS_ANIMATION& animation, ucsl::resources::swif::v6::SRS_MOTION& motion, ucsl::resources::swif::v6::SRS_TRACK& track);
		static void RenderPlotLines(ucsl::resources::swif::v6::SRS_TRACK& track);
		static const char* TrackName(ucsl::resources::swif::v6::ECurveType curveType);
		static const char* TrackName(ucsl::resources::swif::v6::SRS_TRACK& track);

		static constexpr int POINTS_PER_SEGMENT = 20;
		static const char* interpolationTypes[3];

		template<typename T>
		static inline ucsl::resources::swif::v6::Key<T>& GetKeyFrame(ucsl::resources::swif::v6::SRS_TRACK& track, size_t i) {
			switch (track.GetInterpolationType()) {
			case ucsl::resources::swif::v6::EInterpolationType::CONSTANT:
				return reinterpret_cast<ucsl::resources::swif::v6::KeyLinear<T>*>(track.keyFrames.constantFloat)[i];
			case ucsl::resources::swif::v6::EInterpolationType::LINEAR:
				return reinterpret_cast<ucsl::resources::swif::v6::KeyLinear<T>*>(track.keyFrames.linearFloat)[i];
			case ucsl::resources::swif::v6::EInterpolationType::HERMITE:
				return reinterpret_cast<ucsl::resources::swif::v6::KeyHermite<T>*>(track.keyFrames.hermiteFloat)[i];
			case ucsl::resources::swif::v6::EInterpolationType::INDIVIDUAL:
				return reinterpret_cast<ucsl::resources::swif::v6::KeyIndividual<T>*>(track.keyFrames.individualFloat)[i];
			default:
				assert(false);
				return reinterpret_cast<ucsl::resources::swif::v6::KeyLinear<T>*>(track.keyFrames.linearFloat)[i];
			}
		}

		template<typename T>
		static inline double DefaultSelector(const T& x) {
			return static_cast<double>(x);
		}

		template<typename T>
		static inline void DefaultSetter(T& x, double y) {
			x = static_cast<T>(y);
		}

		template<typename T, double (*Selector)(const T& x) = DefaultSelector>
		static ImPlotPoint GeneratePlotLine(int i, void* userData) {
			auto& track = *static_cast<ucsl::resources::swif::v6::SRS_TRACK*>(userData);

			if (i-- == 0)
				return ImPlotPoint(track.firstFrame, Selector(GetKeyFrame<T>(track, 0).value));

			auto segment = i / POINTS_PER_SEGMENT;
			auto point = i % POINTS_PER_SEGMENT;
			auto interpType = track.GetInterpolationType() == ucsl::resources::swif::v6::EInterpolationType::INDIVIDUAL ? static_cast<ucsl::resources::swif::v6::KeyIndividual<T>&>(GetKeyFrame<T>(track, segment)).interpolationType : static_cast<ucsl::resources::swif::v6::EInterpolationType>(track.GetInterpolationType());
			auto& kf = GetKeyFrame<T>(track, segment);

			if (segment == track.keyCount - 1)
				return ImPlotPoint(point == 0 ? kf.frame : track.lastFrame, Selector(kf.value));

			auto& nextKf = GetKeyFrame<T>(track, segment + 1);

			auto t = static_cast<double>(point) / static_cast<double>(POINTS_PER_SEGMENT - 1);
			auto dx = static_cast<double>(nextKf.frame - kf.frame);

			switch (interpType) {
			case ucsl::resources::swif::v6::EInterpolationType::CONSTANT:
				return ImPlotPoint(kf.frame + dx * t, Selector(kf.value));
			case ucsl::resources::swif::v6::EInterpolationType::LINEAR:
				return ImPlotPoint(kf.frame + dx * t, Selector(kf.value) + (Selector(nextKf.value) - Selector(kf.value)) * t);
			case ucsl::resources::swif::v6::EInterpolationType::HERMITE:
				return ImPlotPoint(
					kf.frame + dx * t,
					Selector(kf.value) * (1.0 + 2.0 * t) * (1.0 - t) * (1.0 - t) +
					Selector(static_cast<ucsl::resources::swif::v6::KeyHermite<T>&>(kf).derivativeOut) * (dx * t) * (1.0 - t) * (1.0 - t) +
					Selector(nextKf.value) * (1.0 + 2.0 * (1.0 - t)) * t * t +
					Selector(static_cast<ucsl::resources::swif::v6::KeyHermite<T>&>(nextKf).derivativeIn) * (dx * (t - 1.0)) * t * t
				);
			default:
				assert(false);
				return ImPlotPoint(0, 0);
			}
		}

		template<typename T>
		static void RenderValueEditor(const char* name, T& value, float step) {
			ImGui::DragScalar(name, imgui_datatype<T>::dtype, &value, step);
		}

		template<>
		void RenderValueEditor<bool>(const char* name, bool& value, float step) {
			ImGui::Checkbox(name, &value);
		}

		template<>
		void RenderValueEditor<ucsl::resources::swif::v6::Color>(const char* name, ucsl::resources::swif::v6::Color& value, float step) {
			float colorAsFloat[]{
				static_cast<float>(value.r) / 255,
				static_cast<float>(value.g) / 255,
				static_cast<float>(value.b) / 255,
				static_cast<float>(value.a) / 255,
			};
			float editableColor[4]{ colorAsFloat[0], colorAsFloat[1], colorAsFloat[2], colorAsFloat[3] };

			if (ImGui::ColorEdit4("Value", editableColor, ImGuiColorEditFlags_Uint8)) {
				value.r = static_cast<uint8_t>(editableColor[0] * 255);
				value.g = static_cast<uint8_t>(editableColor[1] * 255);
				value.b = static_cast<uint8_t>(editableColor[2] * 255);
				value.a = static_cast<uint8_t>(editableColor[3] * 255);
			}
		}

		template<typename T>
		static void RenderKeyFrameEditor(ucsl::resources::swif::v6::SRS_TRACK& track, ucsl::resources::swif::v6::SRS_KEYFRAME& keyFrame)
		{
			auto trackInterpType = track.GetInterpolationType();

			RenderValueEditor("Value", static_cast<ucsl::resources::swif::v6::Key<T>&>(keyFrame).value, 0.01f);

			if (trackInterpType != ucsl::resources::swif::v6::EInterpolationType::CONSTANT && trackInterpType != ucsl::resources::swif::v6::EInterpolationType::LINEAR) {
				ImGui::SeparatorText("Interpolation");
				if (trackInterpType == ucsl::resources::swif::v6::EInterpolationType::INDIVIDUAL)
					ComboEnum("Interpolation type", static_cast<ucsl::resources::swif::v6::KeyIndividual<float>&>(keyFrame).interpolationType, interpolationTypes);

				RenderValueEditor("Left derivative", static_cast<ucsl::resources::swif::v6::KeyHermite<T>&>(keyFrame).derivativeIn, 0.01f);
				RenderValueEditor("Right derivative", static_cast<ucsl::resources::swif::v6::KeyHermite<T>&>(keyFrame).derivativeOut, 0.01f);
			}
		}

		static void SetupYAxis(double min, double max) {
			ImPlot::SetupAxisLimits(ImAxis_Y1, min, max, ImPlotCond_Always);
		}

		template<typename T>
		static void SetupFloatingYAxis(ucsl::resources::swif::v6::SRS_TRACK& track) {
			T min = std::numeric_limits<T>::max(), max = std::numeric_limits<T>::lowest();

			for (size_t i = 0; i < track.keyCount; i++) {
				T val = GetKeyFrame<T>(track, i).value;
				min = std::min(min, val);
				max = std::max(max, val);
			}

			SetupYAxis(static_cast<double>(min) - 10.0, static_cast<double>(max) + 10.0);
		}

		template<typename T, double (*Selector)(const T& x) = DefaultSelector, void (*Setter)(T& x, double y) = DefaultSetter>
		static void RenderDragPoints(ucsl::resources::swif::v6::SRS_TRACK& track, double min, double max, const char* id = "Points", ImVec4 color = ImVec4(0.31f, 0.69f, 0.776f, 1.0f)) {
			ImGui::PushID(id);
			for (unsigned short i = 0; i < track.keyCount; i++) {
				auto& kf = GetKeyFrame<T>(track, i);
				double frame = kf.frame;
				double value = Selector(kf.value);
				bool clicked;
				bool hovered;
				bool held;

				ImGui::PushID(&kf);
				if (ImPlot::DragPoint(i, &frame, &value, color, 4.0f, 0, &clicked, &hovered, &held)) {
					frame = std::clamp(frame, static_cast<double>(i == 0 ? track.firstFrame : GetKeyFrame<T>(track, i - 1).frame + 1), static_cast<double>(i == track.keyCount - 1 ? track.lastFrame : GetKeyFrame<T>(track, i + 1).frame - 1));
					value = std::clamp(value, min, max);

					kf.frame = static_cast<uint32_t>(std::round(frame));
					Setter(kf.value, value);
				}

				if (hovered || held) {
					ImGui::SetTooltip("Frame: %d\nValue: %f", kf.frame, value);
				}

				if (clicked && !ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
					ImGui::OpenPopup("Editor");
				}

				if (ImGui::BeginPopup("Editor")) {
					RenderKeyFrameEditor<T>(track, kf);
					ImGui::EndPopup();
				}
				ImGui::PopID();
			}
			ImGui::PopID();
		}

		template<typename T, double (*Selector)(const T& x) = DefaultSelector, void (*Setter)(T& x, double y) = DefaultSetter>
		static void RenderDragPoints(ucsl::resources::swif::v6::SRS_TRACK& track, const char* id = "Points", ImVec4 color = ImVec4(0.31f, 0.69f, 0.776f, 1.0f)) {
			RenderDragPoints<T, Selector, Setter>(track, static_cast<double>(std::numeric_limits<T>::lowest()), static_cast<double>(std::numeric_limits<T>::max()), id, color);
		}

		template<typename T, double (*Selector)(const T& x) = DefaultSelector>
		static void RenderPlotLine(ucsl::resources::swif::v6::SRS_TRACK& track, const char* name = "Value", ImVec4 color = ImVec4(0.31f, 0.69f, 0.776f, 1.0f), bool shaded = true) {
			ImPlot::SetNextLineStyle(color);
			if (shaded)
				ImPlot::SetNextFillStyle(color, 0.3f);
			ImPlot::PlotLineG("R", GeneratePlotLine<T, Selector>, &track, (track.keyCount - 1) * POINTS_PER_SEGMENT + 3, shaded ? ImPlotLineFlags_Shaded : ImPlotLineFlags_None);
		}
	};
}
