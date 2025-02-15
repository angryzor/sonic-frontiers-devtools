#pragma once
#include <ui/operation-modes/Panel.h>
#include <imtimeline.h>
#include "../Context.h"
#include <array>
#include <optional>

namespace ui::operation_modes::modes::dvscene_editor {
	class Timeline : public Panel<Context> {
    private:
        ImTimeline::ImTimelineContext* timelineCtx;
		bool playing;
	public:

        Timeline(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode);
		virtual ~Timeline();

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;

		void SetTimescale(float scale);
		float GetTimescale();
		void RenderTimeline(int& start, int& end, float* curve = nullptr, int size = 0, bool axisLimit = true, float maxValue = 1.0f);
	};

	template<int type>
	void RenderNodeTimeline(Timeline* timeline, hh::dv::DvNodeBase* node) {}

#ifdef DEVTOOLS_TARGET_SDK_rangers
	template<int type>
	void RenderElementTimeline(Timeline* timeline, hh::dv::DvNodeElement* element) {}
#endif
}
