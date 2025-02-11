#include "SurfRideElement.h"
#include <span>

namespace ui::operation_modes::modes::surfride_editor {
	using namespace ucsl::resources::swif::v6;

	SurfRideElement::SurfRideElement(const SRS_SCENE& scene, const SurfRide::Scene* runtimeScene)
		: type{ Type::SCENE }, path{ BuildScenePath(scene) }, runtimePath{ runtimeScene == nullptr ? std::nullopt : std::make_optional(BuildRuntimeScenePath(*runtimeScene)) } {}

	SurfRideElement::SurfRideElement(const SRS_SCENE& scene, const SRS_CAMERA& camera, const SurfRide::Camera* runtimeCamera)
		: type{ Type::CAMERA }, path{ BuildCameraPath(scene, camera) }, runtimePath{ runtimeCamera == nullptr ? std::nullopt : std::make_optional(BuildRuntimeCameraPath(*runtimeCamera)) } {}

	SurfRideElement::SurfRideElement(const SRS_SCENE& scene, const SRS_LAYER& layer, const SurfRide::Layer* runtimeLayer)
		: type{ Type::LAYER }, path{ BuildLayerPath(scene, layer) }, runtimePath{ runtimeLayer == nullptr ? std::nullopt : std::make_optional(BuildRuntimeLayerPath(*runtimeLayer)) } {}

	SurfRideElement::SurfRideElement(const SRS_SCENE& scene, const SRS_LAYER& layer, const SRS_CASTNODE& cast, const SurfRide::Cast* runtimeCast)
		: type{ Type::CAST }, path{ BuildCastPath(scene, layer, cast) }, runtimePath{ runtimeCast == nullptr ? std::nullopt : std::make_optional(BuildRuntimeCastPath(*runtimeCast)) } {}

	SurfRideElement::SurfRideElement(const SurfRide::Scene& runtimeScene)
		: SurfRideElement{ *runtimeScene.sceneData, &runtimeScene } {}

	SurfRideElement::SurfRideElement(const SurfRide::Layer& runtimeLayer)
		: SurfRideElement{ *runtimeLayer.scene->sceneData, *runtimeLayer.layerData, &runtimeLayer } {}

	SurfRideElement::SurfRideElement(const SurfRide::Cast& runtimeCast)
		: SurfRideElement{ *runtimeCast.layer->scene->sceneData, *runtimeCast.layer->layerData, *runtimeCast.castData, &runtimeCast } {}

	bool SurfRideElement::operator==(const SurfRideElement& other) const {
		return type == other.type && path == other.path && runtimePath == other.runtimePath;
	}

	ucsl::resources::swif::v6::SRS_SCENE* ResolveScenePathFragment(ucsl::resources::swif::v6::SRS_PROJECT& project, const SurfRideElementPathFragment& path) {
		for (auto& scene : std::span(project.scenes, project.sceneCount))
			if (path == scene.name)
				return &scene;

		return nullptr;
	}

	ucsl::resources::swif::v6::SRS_CAMERA* ResolveCameraPathFragment(ucsl::resources::swif::v6::SRS_SCENE& scene, const SurfRideElementPathFragment& path) {
		for (auto& camera : std::span(scene.cameras, scene.cameraCount))
			if (path == camera.name)
				return &camera;

		return nullptr;
	}

	ucsl::resources::swif::v6::SRS_LAYER* ResolveLayerPathFragment(ucsl::resources::swif::v6::SRS_SCENE& scene, const SurfRideElementPathFragment& path) {
		for (auto& layer : std::span(scene.layers, scene.layerCount))
			if (path == layer.name)
				return &layer;

		return nullptr;
	}

	ucsl::resources::swif::v6::SRS_CASTNODE* ResolveCastPathFragment(ucsl::resources::swif::v6::SRS_LAYER& layer, const SurfRideElementPathFragment& path) {
		for (auto& cast : std::span(layer.casts, layer.castCount))
			if (path == cast.name)
				return &cast;

		return nullptr;
	}

	SurfRide::Scene* ResolveRuntimeScenePathFragment(SurfRide::Project& project, const SurfRideElementPathFragment& path) {
		return project.GetScene(path.c_str());
	}

	SurfRide::Camera* ResolveRuntimeCameraPathFragment(SurfRide::Scene& scene, const SurfRideElementPathFragment& path) {
		return nullptr;
	}

	SurfRide::Layer* ResolveRuntimeLayerPathFragment(SurfRide::Scene& scene, const SurfRideElementPathFragment& path) {
		return scene.GetLayer(path.c_str());
	}

	SurfRide::Layer* ResolveRuntimeLayerPathFragment(SurfRide::ReferenceCast& cast, const SurfRideElementPathFragment& path) {
		return path == cast.refLayer->name.c_str() ? cast.refLayer : nullptr;
	}

	SurfRide::Cast* ResolveRuntimeCastPathFragment(SurfRide::Layer& layer, const SurfRideElementPathFragment& path) {
		return layer.GetCast(path.c_str());
	}

	SurfRideElementPathFragment BuildScenePathFragment(const ucsl::resources::swif::v6::SRS_SCENE& scene) {
		return scene.name;
	}

	SurfRideElementPathFragment BuildCameraPathFragment(const ucsl::resources::swif::v6::SRS_CAMERA& camera) {
		return camera.name;
	}

	SurfRideElementPathFragment BuildLayerPathFragment(const ucsl::resources::swif::v6::SRS_LAYER& layer) {
		return layer.name;
	}

	SurfRideElementPathFragment BuildCastPathFragment(const ucsl::resources::swif::v6::SRS_CASTNODE& cast) {
		return cast.name;
	}

	SurfRideElementPathFragment BuildRuntimeScenePathFragment(const SurfRide::Scene& scene) {
		return scene.sceneData->name;
	}

	SurfRideElementPathFragment BuildRuntimeCameraPathFragment(const SurfRide::Camera& camera) {
		return camera.camera.name;
	}

	SurfRideElementPathFragment BuildRuntimeLayerPathFragment(const SurfRide::Layer& layer) {
		return layer.name.c_str();
	}

	SurfRideElementPathFragment BuildRuntimeCastPathFragment(const SurfRide::Cast& cast) {
		return cast.castData->name;
	}

	ucsl::resources::swif::v6::SRS_SCENE* ResolveScenePath(ucsl::resources::swif::v6::SRS_PROJECT& project, const SurfRideElementPath& path) {
		if (path.size() != 1)
			return nullptr;

		return ResolveScenePathFragment(project, path[0]);
	}

	ucsl::resources::swif::v6::SRS_CAMERA* ResolveCameraPath(ucsl::resources::swif::v6::SRS_PROJECT& project, const SurfRideElementPath& path) {
		if (path.size() != 2)
			return nullptr;

		auto* scene = ResolveScenePathFragment(project, path[0]);
		
		return scene == nullptr ? nullptr : ResolveCameraPathFragment(*scene, path[1]);
	}

	ucsl::resources::swif::v6::SRS_LAYER* ResolveLayerPath(ucsl::resources::swif::v6::SRS_PROJECT& project, const SurfRideElementPath& path) {
		if (path.size() != 2)
			return nullptr;

		auto* scene = ResolveScenePathFragment(project, path[0]);
		
		return scene == nullptr ? nullptr : ResolveLayerPathFragment(*scene, path[1]);
	}

	ucsl::resources::swif::v6::SRS_CASTNODE* ResolveCastPath(ucsl::resources::swif::v6::SRS_PROJECT& project, const SurfRideElementPath& path) {
		if (path.size() != 3)
			return nullptr;

		auto* scene = ResolveScenePathFragment(project, path[0]);
		auto* layer = ResolveLayerPathFragment(*scene, path[1]);
		
		return layer == nullptr ? nullptr : ResolveCastPathFragment(*layer, path[2]);
	}

	SurfRide::Scene* ResolveRuntimeScenePath(SurfRide::Project& project, const SurfRideElementPath& path) {
		if (path.size() != 1)
			return nullptr;

		return ResolveRuntimeScenePathFragment(project, path[0]);
	}

	SurfRide::Camera* ResolveRuntimeCameraPath(SurfRide::Project& project, const SurfRideElementPath& path) {
		if (path.size() != 2)
			return nullptr;

		auto* scene = ResolveRuntimeScenePathFragment(project, path[0]);
		
		return scene == nullptr ? nullptr : ResolveRuntimeCameraPathFragment(*scene, path[1]);
	}

	SurfRide::Layer* ResolveRuntimeLayerPath(SurfRide::Project& project, const SurfRideElementPath& path) {
		size_t i{};
		if (path.size() < 3)
			return nullptr;

		auto* scene = ResolveRuntimeScenePathFragment(project, path[i++]);
		auto* layer = scene == nullptr ? nullptr : ResolveRuntimeLayerPathFragment(*scene, path[i++]);

		while (layer != nullptr && i < path.size()) {
			auto* cast = ResolveRuntimeCastPathFragment(*layer, path[i++]);

			if (cast == nullptr || i >= path.size() || cast->castData->GetType() != ucsl::resources::swif::v6::SRS_CASTNODE::Type::REFERENCE)
				return nullptr;

			layer = ResolveRuntimeLayerPathFragment(*static_cast<SurfRide::ReferenceCast*>(cast), path[i++]);
		}

		return layer;
	}

	SurfRide::Cast* ResolveRuntimeCastPath(SurfRide::Project& project, const SurfRideElementPath& path) {
		size_t i{};
		if (path.size() < 3)
			return nullptr;

		auto* scene = ResolveRuntimeScenePathFragment(project, path[i++]);
		auto* layer = scene == nullptr ? nullptr : ResolveRuntimeLayerPathFragment(*scene, path[i++]);
		auto* cast = layer == nullptr ? nullptr : ResolveRuntimeCastPathFragment(*layer, path[i++]);

		while (cast != nullptr && i < path.size()) {
			if (cast->castData->GetType() != ucsl::resources::swif::v6::SRS_CASTNODE::Type::REFERENCE)
				return nullptr;

			layer = ResolveRuntimeLayerPathFragment(*static_cast<SurfRide::ReferenceCast*>(cast), path[i++]);

			if (layer == nullptr || i >= path.size())
				return nullptr;

			cast = ResolveRuntimeCastPathFragment(*layer, path[i++]);
		}

		return cast;
	}

	SurfRideElementPath BuildScenePath(const ucsl::resources::swif::v6::SRS_SCENE& scene) {
		SurfRideElementPath res{};
		res.push_back(BuildScenePathFragment(scene));
		return res;
	}

	SurfRideElementPath BuildCameraPath(const ucsl::resources::swif::v6::SRS_SCENE& scene, const ucsl::resources::swif::v6::SRS_CAMERA& camera) {
		SurfRideElementPath res{};
		res.push_back(BuildScenePathFragment(scene));
		res.push_back(BuildCameraPathFragment(camera));
		return res;
	}

	SurfRideElementPath BuildLayerPath(const ucsl::resources::swif::v6::SRS_SCENE& scene, const ucsl::resources::swif::v6::SRS_LAYER& layer) {
		SurfRideElementPath res{};
		res.push_back(BuildScenePathFragment(scene));
		res.push_back(BuildLayerPathFragment(layer));
		return res;
	}

	SurfRideElementPath BuildCastPath(const ucsl::resources::swif::v6::SRS_SCENE& scene, const ucsl::resources::swif::v6::SRS_LAYER& layer, const ucsl::resources::swif::v6::SRS_CASTNODE& cast) {
		SurfRideElementPath res{};
		res.push_back(BuildScenePathFragment(scene));
		res.push_back(BuildLayerPathFragment(layer));
		res.push_back(BuildCastPathFragment(cast));
		return res;
	}

	void AddScene(const SurfRide::Scene& scene, SurfRideElementPath& path) {
		path.push_back(BuildRuntimeScenePathFragment(scene));
	}

	void AddLayer(const SurfRide::Layer& layer, SurfRideElementPath& path);
	void AddCast(const SurfRide::Cast& cast, SurfRideElementPath& path) {
		AddLayer(*cast.layer, path);
		path.push_back(BuildRuntimeCastPathFragment(cast));
	}

	void AddLayer(const SurfRide::Layer& layer, SurfRideElementPath& path) {
		if (layer.referenceCast)
			AddCast(*layer.referenceCast, path);
		else
			AddScene(*layer.scene, path);

		path.push_back(BuildRuntimeLayerPathFragment(layer));
	}

	SurfRideElementPath BuildRuntimeScenePath(const SurfRide::Scene& scene) {
		SurfRideElementPath res{};
		AddScene(scene, res);
		return res;
	}

	SurfRideElementPath BuildRuntimeCameraPath(const SurfRide::Camera& camera) {
		return SurfRideElementPath{};
	}

	SurfRideElementPath BuildRuntimeLayerPath(const SurfRide::Layer& layer) {
		SurfRideElementPath res{};
		AddLayer(layer, res);
		return res;
	}

	SurfRideElementPath BuildRuntimeCastPath(const SurfRide::Cast& cast) {
		SurfRideElementPath res{};
		AddCast(cast, res);
		return res;
	}

	bool SurfRideElementPath::operator==(const SurfRideElementPath& other) const {
		if (size() != other.size())
			return false;

		for (size_t i = 0; i < size(); i++)
			if ((*this)[i] != other[i])
				return false;

		return true;
	}

	bool SurfRideElementPath::operator!=(const SurfRideElementPath& other) const {
		return !operator==(other);
	}
}
