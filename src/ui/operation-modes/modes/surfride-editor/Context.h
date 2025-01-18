#pragma once
#include <resources/ManagedMemoryRegistry.h>
#include <resources/managed-memory/ManagedCArray.h>
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
		void AddMotion(ucsl::resources::swif::v6::SRS_ANIMATION& animation, ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		void AddTrack(ucsl::resources::swif::v6::SRS_MOTION& motion, ucsl::resources::swif::v6::ECurveType type, unsigned int firstFrame, unsigned int lastFrame);
		void AddKeyFrame(ucsl::resources::swif::v6::SRS_TRACK& track, unsigned int frame);

	private:
		template<typename T>
		inline void AddKeyFrameEx(ucsl::resources::swif::v6::SRS_TRACK& track, unsigned int frame) {
			resources::ManagedCArray<T, unsigned short> keyFrames{ gocSprite->projectResource, reinterpret_cast<T*&>(track.keyFrames), track.keyCount };

			unsigned short i = 0;
			for (auto& kf : keyFrames) {
				if (kf.frame > frame)
					break;

				i++;
			}

			keyFrames.emplace_back();

			for (unsigned short j = i + 1; j < keyFrames.size(); j++)
				keyFrames[j] = keyFrames[j - 1];

			keyFrames[i] = T{};
			keyFrames[i].frame = frame;
		}
	};
}
