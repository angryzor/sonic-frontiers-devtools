#pragma once

namespace ui::operation_modes::modes::surfride_editor {
	using SurfRideElementPathFragment = std::string;
	struct SurfRideElementPath : std::vector<SurfRideElementPathFragment> {
		bool operator==(const SurfRideElementPath& other) const;
		bool operator!=(const SurfRideElementPath& other) const;
	};

	ucsl::resources::swif::v6::SRS_SCENE* ResolveScenePath(ucsl::resources::swif::v6::SRS_PROJECT& project, const SurfRideElementPath& path);
	ucsl::resources::swif::v6::SRS_CAMERA* ResolveCameraPath(ucsl::resources::swif::v6::SRS_PROJECT& project, const SurfRideElementPath& path);
	ucsl::resources::swif::v6::SRS_LAYER* ResolveLayerPath(ucsl::resources::swif::v6::SRS_PROJECT& project, const SurfRideElementPath& path);
	ucsl::resources::swif::v6::SRS_CASTNODE* ResolveCastPath(ucsl::resources::swif::v6::SRS_PROJECT& project, const SurfRideElementPath& path);
	SurfRide::Scene* ResolveRuntimeScenePath(SurfRide::Project& project, const SurfRideElementPath& path);
	SurfRide::Camera* ResolveRuntimeCameraPath(SurfRide::Project& project, const SurfRideElementPath& path);
	SurfRide::Layer* ResolveRuntimeLayerPath(SurfRide::Project& project, const SurfRideElementPath& path);
	SurfRide::Cast* ResolveRuntimeCastPath(SurfRide::Project& project, const SurfRideElementPath& path);

	SurfRideElementPath BuildScenePath(const ucsl::resources::swif::v6::SRS_SCENE& scene);
	SurfRideElementPath BuildCameraPath(const ucsl::resources::swif::v6::SRS_SCENE& scene, const ucsl::resources::swif::v6::SRS_CAMERA& camera);
	SurfRideElementPath BuildLayerPath(const ucsl::resources::swif::v6::SRS_SCENE& scene, const ucsl::resources::swif::v6::SRS_LAYER& layer);
	SurfRideElementPath BuildCastPath(const ucsl::resources::swif::v6::SRS_SCENE& scene, const ucsl::resources::swif::v6::SRS_LAYER& layer, const ucsl::resources::swif::v6::SRS_CASTNODE& cast);
	SurfRideElementPath BuildRuntimeScenePath(const SurfRide::Scene& scene);
	SurfRideElementPath BuildRuntimeCameraPath(const SurfRide::Camera& camera);
	SurfRideElementPath BuildRuntimeLayerPath(const SurfRide::Layer& layer);
	SurfRideElementPath BuildRuntimeCastPath(const SurfRide::Cast& cast);

	struct SurfRideElement {
		enum class Type {
			SCENE,
			CAMERA,
			LAYER,
			CAST,
			//TEXTURE,
		};

		Type type{};
		SurfRideElementPath path{};
		std::optional<SurfRideElementPath> runtimePath{};

		SurfRideElement(const ucsl::resources::swif::v6::SRS_SCENE& scene, const SurfRide::Scene* runtimeScene);
		SurfRideElement(const ucsl::resources::swif::v6::SRS_SCENE& scene, const ucsl::resources::swif::v6::SRS_CAMERA& camera, const SurfRide::Camera* runtimeCamera);
		SurfRideElement(const ucsl::resources::swif::v6::SRS_SCENE& scene, const ucsl::resources::swif::v6::SRS_LAYER& layer, const SurfRide::Layer* runtimeLayer);
		SurfRideElement(const ucsl::resources::swif::v6::SRS_SCENE& scene, const ucsl::resources::swif::v6::SRS_LAYER& layer, const ucsl::resources::swif::v6::SRS_CASTNODE& cast, const SurfRide::Cast* runtimeCast);
		SurfRideElement(const SurfRide::Scene& runtimeScene);
		//SurfRideElement(const SurfRide::Camera& runtimeCamera);
		SurfRideElement(const SurfRide::Layer& runtimeLayer);
		SurfRideElement(const SurfRide::Cast& runtimeCast);

		bool operator==(const SurfRideElement& other) const;
	};
}
