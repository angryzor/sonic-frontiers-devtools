#pragma once
#include <utilities/CompatibleObject.h>
#include <random>
#include "SurfRideElement.h"

namespace ui::operation_modes::modes::surfride_editor {
	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;

		hh::fnd::Reference<hh::ui::GOCSprite> gocSprite{};
		SurfRide::Scene* focusedScene{};
		std::mt19937 mt{ std::random_device{}() };

		csl::math::Matrix44 GetFocusedSceneCameraMatrix() const;
		static csl::math::Matrix44 GetSceneCameraMatrix(const SurfRide::Scene* scene);
		static csl::math::Matrix44 GetFullCastTransform(const SurfRide::Cast* cast);

		void CreateImageCast(SurfRide::Cast* sibling);
		void AddImageCast(SurfRide::Layer* layer);
		void AddImageCast(SurfRide::Cast* parent);
	};
}
