#pragma once
#include <utilities/CompatibleObject.h>

namespace ui::operation_modes::modes::surfride_editor {
	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;

		hh::fnd::Reference<hh::ui::GOCSprite> gocSprite{};
		SurfRide::Scene* focusedScene{};

		csl::math::Matrix44 GetFocusedSceneCameraMatrix() const;
		static csl::math::Matrix44 GetSceneCameraMatrix(const SurfRide::Scene* scene);
		static csl::math::Matrix44 GetFullCastTransform(const SurfRide::Cast* cast);
	};
}
