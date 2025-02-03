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
		hh::fnd::Reference<hh::ui::ResSurfRideProject> projectResource{};
		ucsl::resources::swif::v6::SRS_PROJECT* project{};
		ucsl::resources::swif::v6::SRS_SCENE* focusedScene{};
		std::mt19937 mt{ std::random_device{}() };

		csl::math::Matrix44 GetFocusedSceneCameraMatrix() const;
		static csl::math::Matrix44 GetSceneCameraMatrix(const SurfRide::Scene* scene);
		static csl::math::Matrix44 GetFullCastTransform(const SurfRide::Cast* cast);

		void CreateImageCast(ucsl::resources::swif::v6::SRS_LAYER& layer, ucsl::resources::swif::v6::SRS_CASTNODE& sibling);
		void AddImageCast(ucsl::resources::swif::v6::SRS_LAYER& layer);
		void AddImageCast(ucsl::resources::swif::v6::SRS_CASTNODE& parent);
		void AddMotion(ucsl::resources::swif::v6::SRS_ANIMATION& animation, ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		void AddTrack(ucsl::resources::swif::v6::SRS_MOTION& motion, ucsl::resources::swif::v6::ECurveType type, unsigned int firstFrame, unsigned int lastFrame);
		void AddKeyFrame(ucsl::resources::swif::v6::SRS_TRACK& track, unsigned int frame);
		void SetTrackStart(ucsl::resources::swif::v6::SRS_TRACK& track, unsigned int frame);
		void SetTrackEnd(ucsl::resources::swif::v6::SRS_TRACK& track, unsigned int frame);
		void MoveTrack(ucsl::resources::swif::v6::SRS_TRACK& track, int delta);

		template<typename F>
		void VisitElement(const SurfRideElement& element, F f) {
			switch (element.type) {
			case SurfRideElement::Type::SCENE: f(FindScene(element.id));
			case SurfRideElement::Type::CAMERA: f(FindCamera(element.id));
			case SurfRideElement::Type::LAYER: f(FindLayer(element.id));
			case SurfRideElement::Type::CAST: f(FindCast(element.id));
			default: f(FindScene(element.id));
			}
		}

		ucsl::resources::swif::v6::SRS_SCENE* FindScene(int id) const;
		ucsl::resources::swif::v6::SRS_LAYER* FindLayer(ucsl::resources::swif::v6::SRS_SCENE& scene, int id) const;
		ucsl::resources::swif::v6::SRS_LAYER* FindLayer(int id) const;
		ucsl::resources::swif::v6::SRS_CAMERA* FindCamera(ucsl::resources::swif::v6::SRS_SCENE& scene, int id) const;
		ucsl::resources::swif::v6::SRS_CAMERA* FindCamera(int id) const;
		ucsl::resources::swif::v6::SRS_CASTNODE* FindCast(ucsl::resources::swif::v6::SRS_LAYER& layer, int id) const;
		ucsl::resources::swif::v6::SRS_CASTNODE* FindCast(ucsl::resources::swif::v6::SRS_SCENE& scene, int id) const;
		ucsl::resources::swif::v6::SRS_CASTNODE* FindCast(int id) const;
		
		ucsl::resources::swif::v6::SRS_LAYER* FindCastLayer(int castId) const;
		ucsl::resources::swif::v6::SRS_CASTNODE& FindLastSibling(const ucsl::resources::swif::v6::SRS_LAYER& layer, ucsl::resources::swif::v6::SRS_CASTNODE& sibling) const;
		
		template<typename F>
		void ForEachSibling(const ucsl::resources::swif::v6::SRS_LAYER& layer, int siblingIndex, F f) {
			while (siblingIndex != -1) {
				auto& cast = layer.casts[siblingIndex];

				f(cast);
				siblingIndex = cast.siblingIndex;
			}
		}

		template<typename F>
		void ForEachChild(const ucsl::resources::swif::v6::SRS_LAYER& layer, const ucsl::resources::swif::v6::SRS_CASTNODE& parent, F f) {
			ForEachSibling(layer, parent.childIndex, std::forward<F>(f));
		}

		template<typename F>
		void ForEachRoot(const ucsl::resources::swif::v6::SRS_LAYER& layer, F f) {
			if (layer.castCount > 0)
				ForEachSibling(layer, 0, std::forward<F>(f));
		}

		template<typename F>
		void VisitKeyFrames(const ucsl::resources::swif::v6::SRS_TRACK& track, F f) {
			switch (track.GetInterpolationType()) {
			case ucsl::resources::swif::v6::EInterpolationType::CONSTANT:
				switch (track.GetDataType()) {
				case ucsl::resources::swif::v6::ETrackDataType::FLOAT: f(track.keyFrames.constantFloat);
				case ucsl::resources::swif::v6::ETrackDataType::INDEX: f(track.keyFrames.constantIndex);
				case ucsl::resources::swif::v6::ETrackDataType::INT: f(track.keyFrames.constantInt);
				case ucsl::resources::swif::v6::ETrackDataType::BOOL: f(track.keyFrames.constantBool);
				case ucsl::resources::swif::v6::ETrackDataType::COLOR: f(track.keyFrames.constantColor);
				default: assert(false && "Invalid track flags");
				}
			case ucsl::resources::swif::v6::EInterpolationType::LINEAR:
				switch (track.GetDataType()) {
				case ucsl::resources::swif::v6::ETrackDataType::FLOAT: f(track.keyFrames.linearFloat);
				case ucsl::resources::swif::v6::ETrackDataType::INDEX: f(track.keyFrames.linearIndex);
				case ucsl::resources::swif::v6::ETrackDataType::INT: f(track.keyFrames.linearInt);
				case ucsl::resources::swif::v6::ETrackDataType::BOOL: f(track.keyFrames.linearBool);
				case ucsl::resources::swif::v6::ETrackDataType::COLOR: f(track.keyFrames.linearColor);
				default: assert(false && "Invalid track flags");
				}
			case ucsl::resources::swif::v6::EInterpolationType::HERMITE:
				switch (track.GetDataType()) {
				case ucsl::resources::swif::v6::ETrackDataType::FLOAT: f(track.keyFrames.hermiteFloat);
				case ucsl::resources::swif::v6::ETrackDataType::INDEX: f(track.keyFrames.hermiteIndex);
				case ucsl::resources::swif::v6::ETrackDataType::INT: f(track.keyFrames.hermiteInt);
				case ucsl::resources::swif::v6::ETrackDataType::BOOL: f(track.keyFrames.hermiteBool);
				case ucsl::resources::swif::v6::ETrackDataType::COLOR: f(track.keyFrames.hermiteColor);
				default: assert(false && "Invalid track flags");
				}
			case ucsl::resources::swif::v6::EInterpolationType::INDIVIDUAL:
				switch (track.GetDataType()) {
				case ucsl::resources::swif::v6::ETrackDataType::FLOAT: f(track.keyFrames.individualFloat);
				case ucsl::resources::swif::v6::ETrackDataType::INDEX: f(track.keyFrames.individualIndex);
				case ucsl::resources::swif::v6::ETrackDataType::INT: f(track.keyFrames.individualInt);
				case ucsl::resources::swif::v6::ETrackDataType::BOOL: f(track.keyFrames.individualBool);
				case ucsl::resources::swif::v6::ETrackDataType::COLOR: f(track.keyFrames.individualColor);
				default: assert(false && "Invalid track flags");
				}
			default: assert(false && "Invalid track flags");
			}
		}

		// Live feedback
		SurfRide::Scene* FindRuntimeScene(unsigned int id) const;
		SurfRide::Layer* FindRuntimeLayer(unsigned int id) const;
		SurfRide::Cast* FindRuntimeCast(unsigned int id) const;
		void StartAnimationByIndex(const ucsl::resources::swif::v6::SRS_LAYER& layer, int animationIndex);
		float GetAnimationFrame(const ucsl::resources::swif::v6::SRS_LAYER& layer) const;
		void SetAnimationFrame(const ucsl::resources::swif::v6::SRS_LAYER& layer, float frame);
		void ApplyTransformChange(const ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		void ApplyImageCastChange(const ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		void ApplyReferenceCastChange(const ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		void ApplySliceCastChange(const ucsl::resources::swif::v6::SRS_CASTNODE& cast);

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
