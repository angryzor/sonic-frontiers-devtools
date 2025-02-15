#pragma once
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::surfride_editor
{
	class SurfRideEditor : public OperationMode<Context>, public hh::game::ComponentListener {
	public:
		SurfRideEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host);
		virtual ~SurfRideEditor();

		virtual void ProcessAction(const ActionBase& action) override;
		virtual void RenderScene() override;
		virtual void ComponentRemovedCallback(hh::game::GOComponent* component) override;

		std::optional<csl::geom::Aabb> RenderCasts(SurfRide::Cast& cast);
		void RenderCasts(SurfRide::Layer& layer);
		void RenderCasts(SurfRide::Scene& scene);
	};
}
