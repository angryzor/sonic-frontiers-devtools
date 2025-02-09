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
		
		void SetResource(hh::ui::GOCSprite* resource);
		void AddCast(ucsl::resources::swif::v6::SRS_LAYER& layer, ucsl::resources::swif::v6::SRS_CASTNODE::Type type);
		void AddCast(ucsl::resources::swif::v6::SRS_CASTNODE& parent, ucsl::resources::swif::v6::SRS_CASTNODE::Type type);
		void RemoveCast(ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		void AddMotion(ucsl::resources::swif::v6::SRS_ANIMATION& animation, ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		void AddTrack(ucsl::resources::swif::v6::SRS_MOTION& motion, ucsl::resources::swif::v6::ECurveType type, unsigned int firstFrame, unsigned int lastFrame);
		void AddKeyFrame(ucsl::resources::swif::v6::SRS_TRACK& track, unsigned int frame);
		void RemoveMotion(ucsl::resources::swif::v6::SRS_ANIMATION& animation, ucsl::resources::swif::v6::SRS_MOTION& motion);
		void RemoveTrack(ucsl::resources::swif::v6::SRS_MOTION& motion, ucsl::resources::swif::v6::SRS_TRACK& track);
		void RemoveKeyFrame(ucsl::resources::swif::v6::SRS_TRACK& track, unsigned int frameIdx);
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

		ucsl::resources::swif::v6::SRS_SCENE* FindScene(unsigned int id) const;
		ucsl::resources::swif::v6::SRS_LAYER* FindLayer(ucsl::resources::swif::v6::SRS_SCENE& scene, unsigned int id) const;
		ucsl::resources::swif::v6::SRS_LAYER* FindLayer(unsigned int id) const;
		ucsl::resources::swif::v6::SRS_CAMERA* FindCamera(ucsl::resources::swif::v6::SRS_SCENE& scene, unsigned int id) const;
		ucsl::resources::swif::v6::SRS_CAMERA* FindCamera(unsigned int id) const;
		ucsl::resources::swif::v6::SRS_CASTNODE* FindCast(ucsl::resources::swif::v6::SRS_LAYER& layer, unsigned int id) const;
		ucsl::resources::swif::v6::SRS_CASTNODE* FindCast(ucsl::resources::swif::v6::SRS_SCENE& scene, unsigned int id) const;
		ucsl::resources::swif::v6::SRS_CASTNODE* FindCast(unsigned int id) const;
		
		ucsl::resources::swif::v6::SRS_LAYER* FindCastLayer(unsigned int castId) const;
		ucsl::resources::swif::v6::SRS_CASTNODE* FindCastParent(unsigned int castId) const;
		ucsl::resources::swif::v6::SRS_CASTNODE& FindLastSibling(const ucsl::resources::swif::v6::SRS_LAYER& layer, int sibling) const;
		
		template<typename F>
		static void ForEachSibling(const ucsl::resources::swif::v6::SRS_LAYER& layer, int siblingIndex, F f) {
			while (siblingIndex != -1) {
				auto& cast = layer.casts[siblingIndex];

				f(cast);
				siblingIndex = cast.siblingIndex;
			}
		}

		template<typename F>
		static void ForEachChild(const ucsl::resources::swif::v6::SRS_LAYER& layer, const ucsl::resources::swif::v6::SRS_CASTNODE& parent, F f) {
			ForEachSibling(layer, parent.childIndex, std::forward<F>(f));
		}

		template<typename F>
		static void ForEachRoot(const ucsl::resources::swif::v6::SRS_LAYER& layer, F f) {
			if (layer.castCount > 0)
				ForEachSibling(layer, 0, std::forward<F>(f));
		}

		template<typename F>
		static void VisitKeyFrames(const ucsl::resources::swif::v6::SRS_TRACK& track, F f) {
			switch (track.GetInterpolationType()) {
			case ucsl::resources::swif::v6::EInterpolationType::CONSTANT:
				switch (track.GetDataType()) {
				case ucsl::resources::swif::v6::ETrackDataType::FLOAT: f(track.keyFrames.constantFloat); break;
				case ucsl::resources::swif::v6::ETrackDataType::INDEX: f(track.keyFrames.constantIndex); break;
				case ucsl::resources::swif::v6::ETrackDataType::INT: f(track.keyFrames.constantInt); break;
				case ucsl::resources::swif::v6::ETrackDataType::BOOL: f(track.keyFrames.constantBool); break;
				case ucsl::resources::swif::v6::ETrackDataType::COLOR: f(track.keyFrames.constantColor); break;
				default: assert(false && "Invalid track flags"); break;
				}
			case ucsl::resources::swif::v6::EInterpolationType::LINEAR:
				switch (track.GetDataType()) {
				case ucsl::resources::swif::v6::ETrackDataType::FLOAT: f(track.keyFrames.linearFloat); break;
				case ucsl::resources::swif::v6::ETrackDataType::INDEX: f(track.keyFrames.linearIndex); break;
				case ucsl::resources::swif::v6::ETrackDataType::INT: f(track.keyFrames.linearInt); break;
				case ucsl::resources::swif::v6::ETrackDataType::BOOL: f(track.keyFrames.linearBool); break;
				case ucsl::resources::swif::v6::ETrackDataType::COLOR: f(track.keyFrames.linearColor); break;
				default: assert(false && "Invalid track flags"); break;
				}
			case ucsl::resources::swif::v6::EInterpolationType::HERMITE:
				switch (track.GetDataType()) {
				case ucsl::resources::swif::v6::ETrackDataType::FLOAT: f(track.keyFrames.hermiteFloat); break;
				case ucsl::resources::swif::v6::ETrackDataType::INDEX: f(track.keyFrames.hermiteIndex); break;
				case ucsl::resources::swif::v6::ETrackDataType::INT: f(track.keyFrames.hermiteInt); break;
				case ucsl::resources::swif::v6::ETrackDataType::BOOL: f(track.keyFrames.hermiteBool); break;
				case ucsl::resources::swif::v6::ETrackDataType::COLOR: f(track.keyFrames.hermiteColor); break;
				default: assert(false && "Invalid track flags"); break;
				}
			case ucsl::resources::swif::v6::EInterpolationType::INDIVIDUAL:
				switch (track.GetDataType()) {
				case ucsl::resources::swif::v6::ETrackDataType::FLOAT: f(track.keyFrames.individualFloat); break;
				case ucsl::resources::swif::v6::ETrackDataType::INDEX: f(track.keyFrames.individualIndex); break;
				case ucsl::resources::swif::v6::ETrackDataType::INT: f(track.keyFrames.individualInt); break;
				case ucsl::resources::swif::v6::ETrackDataType::BOOL: f(track.keyFrames.individualBool); break;
				case ucsl::resources::swif::v6::ETrackDataType::COLOR: f(track.keyFrames.individualColor); break;
				default: assert(false && "Invalid track flags"); break;
				}
			default: assert(false && "Invalid track flags"); break;
			}
		}

		// Live feedback
		SurfRide::Scene* FindRuntimeScene(unsigned int id) const;
		template<typename F>
		void ForEachRuntimeLayer(unsigned int id, SurfRide::Cast& cast, F f) const {
			if (cast.castData->GetType() == ucsl::resources::swif::v6::SRS_CASTNODE::Type::REFERENCE)
				if (auto* refLayer = static_cast<SurfRide::ReferenceCast&>(cast).refLayer)
					ForEachRuntimeLayer(id, *refLayer, std::forward<F>(f));

			for (auto child : cast.GetChildren())
				ForEachRuntimeLayer(id, *child, std::forward<F>(f));
		}
		template<typename F>
		void ForEachRuntimeLayer(unsigned int id, SurfRide::Layer& layer, F f) const {
			if (layer.layerData->id == id)
				f(layer);

			for (auto cast : layer.GetCasts())
				ForEachRuntimeLayer(id, *cast, std::forward<F>(f));
		}
		template<typename F>
		void ForEachRuntimeLayer(unsigned int id, SurfRide::Scene& scene, F f) const {
			for (auto layer : scene.GetLayers())
				ForEachRuntimeLayer(id, *layer, std::forward<F>(f));
		}
		template<typename F>
		void ForEachRuntimeLayer(unsigned int id, F f) const {
			if (gocSprite == nullptr)
				return;

			for (auto scene : gocSprite->GetProject()->GetScenes())
				ForEachRuntimeLayer(id, *scene, std::forward<F>(f));
		}
		template<typename F>
		void ForEachRuntimeCast(unsigned int id, SurfRide::Cast& cast, F f) const {
			if (cast.castData->id == id)
				f(cast);

			if (cast.castData->GetType() == ucsl::resources::swif::v6::SRS_CASTNODE::Type::REFERENCE)
				if (auto* refLayer = static_cast<SurfRide::ReferenceCast&>(cast).refLayer)
					ForEachRuntimeCast(id, *refLayer, std::forward<F>(f));

			for (auto child : cast.GetChildren())
				ForEachRuntimeCast(id, *child, std::forward<F>(f));
		}
		template<typename F>
		void ForEachRuntimeCast(unsigned int id, SurfRide::Layer& layer, F f) const {
			for (auto cast : layer.GetCasts())
				ForEachRuntimeCast(id, *cast, std::forward<F>(f));
		}
		template<typename F>
		void ForEachRuntimeCast(unsigned int id, SurfRide::Scene& scene, F f) const {
			for (auto layer : scene.GetLayers())
				ForEachRuntimeCast(id, *layer, std::forward<F>(f));
		}
		template<typename F>
		void ForEachRuntimeCast(unsigned int id, F f) const {
			if (gocSprite == nullptr)
				return;

			for (auto scene : gocSprite->GetProject()->GetScenes())
				ForEachRuntimeCast(id, *scene, std::forward<F>(f));
		}
		template<typename F>
		void ForEachRuntimeCast(SurfRide::Cast& cast, F f) const {
			f(cast);

			if (cast.castData->GetType() == ucsl::resources::swif::v6::SRS_CASTNODE::Type::REFERENCE)
				if (auto* refLayer = static_cast<SurfRide::ReferenceCast&>(cast).refLayer)
					ForEachRuntimeCast(*refLayer, std::forward<F>(f));

			for (auto child : cast.GetChildren())
				ForEachRuntimeCast(*child, std::forward<F>(f));
		}
		template<typename F>
		void ForEachRuntimeCast(SurfRide::Layer& layer, F f) const {
			for (auto cast : layer.GetCasts())
				ForEachRuntimeCast(*cast, std::forward<F>(f));
		}
		template<typename F>
		void ForEachRuntimeCast(SurfRide::Scene& scene, F f) const {
			for (auto layer : scene.GetLayers())
				ForEachRuntimeCast(*layer, std::forward<F>(f));
		}
		template<typename F>
		void ForEachRuntimeCast(F f) const {
			if (gocSprite == nullptr)
				return;

			for (auto scene : gocSprite->GetProject()->GetScenes())
				ForEachRuntimeCast(*scene, std::forward<F>(f));
		}
		SurfRide::Layer* FindRuntimeLayer(unsigned int id) const;
		SurfRide::Cast* FindRuntimeCast(unsigned int id) const;
		static bool UpdateAabbWithoutChildren(const SurfRide::Cast* cast, csl::geom::Aabb& aabb);
		static bool UpdateAabb(const SurfRide::Cast* cast, csl::geom::Aabb& aabb);
		static bool UpdateAabb(const SurfRide::Layer* layer, csl::geom::Aabb& aabb);
		void StartAnimationByIndex(const ucsl::resources::swif::v6::SRS_LAYER& layer, int animationIndex);
		float GetAnimationFrame(const ucsl::resources::swif::v6::SRS_LAYER& layer) const;
		void SetAnimationFrame(const ucsl::resources::swif::v6::SRS_LAYER& layer, float frame);
		void ApplyTransformChange(const ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		void ApplyImageCastChange(const ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		void ApplyReferenceCastChange(const ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		void ApplySliceCastChange(const ucsl::resources::swif::v6::SRS_CASTNODE& cast);
		static Eigen::Translation3f GetPivotTranslation(const SurfRide::Cast* cast);

	private:
		ucsl::resources::swif::v6::SRS_CASTNODE* CreateCast(ucsl::resources::swif::v6::SRS_LAYER& layer, int sibling);
		ucsl::resources::swif::v6::SRS_CASTNODE* CreateImageCast(ucsl::resources::swif::v6::SRS_LAYER& layer, int sibling);
		ucsl::resources::swif::v6::SRS_CASTNODE* CreateSliceCast(ucsl::resources::swif::v6::SRS_LAYER& layer, int sibling);
		ucsl::resources::swif::v6::SRS_CASTNODE* CreateReferenceCast(ucsl::resources::swif::v6::SRS_LAYER& layer, int sibling);
		static ucsl::resources::swif::v6::ETrackDataType GetTrackDataTypeForCurveType(ucsl::resources::swif::v6::ECurveType curveType);
		static Eigen::Translation3f GetPivotTranslation(ucsl::resources::swif::v6::EPivotType pivotType, const ucsl::resources::swif::v6::Vector2& customPivot, const ucsl::resources::swif::v6::Vector2& size);
		static void UpdateAabb(const Eigen::Transform<float, 3, Eigen::Projective>& transform, ucsl::resources::swif::v6::EPivotType pivotType, const ucsl::resources::swif::v6::Vector2& customPivot, const ucsl::resources::swif::v6::Vector2& size, csl::geom::Aabb& aabb);
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
		template<typename T>
		inline void RemoveKeyFrameEx(ucsl::resources::swif::v6::SRS_TRACK& track, size_t frameIndex) {
			resources::ManagedCArray<T, unsigned short> keyFrames{ gocSprite->projectResource, reinterpret_cast<T*&>(track.keyFrames), track.keyCount };

			keyFrames.remove(frameIndex);
		}
	};
}
