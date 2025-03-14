#pragma once
#include <ui/operation-modes/Panel.h>
#include <imtimeline.h>
#include "../Context.h"
#include <array>
#include <optional>

namespace ui::operation_modes::modes::dvscene_editor {
	inline constexpr const char* curveTypeNames[] = {
		"LINEAR",
		"QUADRATIC IN",
		"QUADRATIC OUT",
		"CUBIC",
		"SINE",
		"LOGARITHMIC IN",
		"LOGARITHMIC OUT"
	};

	class Timeline : public Panel<Context> {
    private:
        ImTimeline::ImTimelineContext* timelineCtx;
	public:

        Timeline(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode);
		virtual ~Timeline();

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;

		void SetFrame(float time);
		bool RenderTimeline(int& start, int& end, float* curve = nullptr, int size = 0, bool axisLimit = true, float maxValue = 1.0f);
		bool RenderTimeline(float& start, float& end, float* curve = nullptr, int size = 0, bool axisLimit = true, float maxValue = 1.0f);
		static void GenerateCurve(float* curve, int size, int type, bool decreasing);

		double timelineFalloff = 3.0f;
		bool decreasing = false;
		int curveType = 0;
	};

	template<int type>
	bool RenderNodeTimeline(Timeline* timeline, char* node) {}
}
