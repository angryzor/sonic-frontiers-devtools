#include "Context.h"
#include <span>

namespace ui::operation_modes::modes::surfride_editor {
	using namespace ucsl::resources::swif::v6;

#ifdef DEVTOOLS_TARGET_SDK_wars
	csl::math::Matrix44 Context::GetSceneCameraMatrix(const SurfRide::Scene* scene) {
		auto resolution = SurfRide::GetScreenResolution();
		auto projectionMatrix = csl::math::CreatePerspectiveProjectionMatrix(
			static_cast<float>(scene->camera.camera.cameraData.fov) * 0.000095873802f,
			static_cast<float>(resolution.width) / static_cast<float>(resolution.height),
			scene->camera.camera.cameraData.nearPlane,
			scene->camera.camera.cameraData.farPlane
		);
		return projectionMatrix * scene->camera.viewMatrix;
	}
#else
	csl::math::Matrix44 Context::GetSceneCameraMatrix(const SurfRide::Scene* scene) {
		return scene->camera.projectionMatrix * scene->camera.viewMatrix;
	}
#endif

	csl::math::Matrix44 Context::GetFocusedSceneCameraMatrix() const {
		return GetSceneCameraMatrix(FindRuntimeScene(focusedScene->id));
	}

	csl::math::Matrix44 Context::GetFullCastTransform(const SurfRide::Cast* cast) {
		return GetSceneCameraMatrix(cast->layer->scene) * cast->transform->transformationMatrix;
	}

	void Context::CreateImageCast(SRS_LAYER& layer, SRS_CASTNODE& sibling) {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(gocSprite->projectResource);

		auto* imageCast = new (&managedAllocator) SRS_IMAGECAST{};
		imageCast->flags = 0x00202000;
		imageCast->size = { 100.0f, 100.0f };
		imageCast->SetPivotType(EPivotType::CENTER_CENTER);
		imageCast->vertexColorTopLeft = { 255, 255, 255, 255 };
		imageCast->vertexColorBottomLeft = { 255, 255, 255, 255 };
		imageCast->vertexColorTopRight = { 255, 255, 255, 255 };
		imageCast->vertexColorBottomRight = { 255, 255, 255, 255 };

		resources::ManagedCArray<SRS_CASTNODE, int> casts{ gocSprite->projectResource, layer.casts, layer.castCount };

		auto prevSize = casts.size();

		SRS_CASTNODE newNode{};
		newNode.name = "img_newcast";
		newNode.id = static_cast<int>(mt());
		newNode.flags = 0x750;
		newNode.data.image = imageCast;
		newNode.SetType(SRS_CASTNODE::Type::IMAGE);

		casts.push_back(std::move(newNode));

		// TODO: update live version
		//for (int i = 0; i < prevSize; i++)
		//	layer->casts[i]->castData = (SurfRide::SRS_CASTNODE*)&layer.casts[i];

		FindLastSibling(layer, sibling).siblingIndex = static_cast<short>(prevSize);

		resources::ManagedCArray<SRS_TRS3D, int> transforms{ gocSprite->projectResource, layer.transforms.transforms3d, prevSize };
		transforms.emplace_back();
		transforms[transforms.size() - 1].materialColor = { 255, 255, 255, 255 };
		transforms[transforms.size() - 1].display = true;
	}

	void Context::AddImageCast(SRS_LAYER& layer) {
		CreateImageCast(layer, layer.casts[0]);
	}

	void Context::AddImageCast(SRS_CASTNODE& parent) {
		auto& layer = *FindCastLayer(parent.id);

		CreateImageCast(layer, layer.casts[parent.childIndex]);
	}

	void Context::AddMotion(SRS_ANIMATION& animation, SRS_CASTNODE& cast) {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(gocSprite->projectResource);

		resources::ManagedCArray<SRS_MOTION, unsigned int> motions{ gocSprite->projectResource, animation.motions, animation.motionCount };

		auto& motion = motions.emplace_back();
		motion.castId = static_cast<unsigned short>(cast.id);
		motion.tracks = nullptr;
		motion.trackCount = 0;
	}

	void Context::AddTrack(SRS_MOTION& motion, ECurveType type, unsigned int firstFrame, unsigned int lastFrame) {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(gocSprite->projectResource);

		resources::ManagedCArray<SRS_TRACK, unsigned short> tracks{ gocSprite->projectResource, motion.tracks, motion.trackCount };

		auto& track = tracks.emplace_back();
		track.trackType = type;
		track.flags = (static_cast<unsigned int>(EInterpolationType::LINEAR) & 0x3) | ((static_cast<unsigned int>(ETrackDataType::FLOAT) & 0xF) << 4);
		track.firstFrame = firstFrame;
		track.lastFrame = lastFrame;
		track.keyFrames.constantBool = nullptr;
		track.keyCount = 0;
	}

	void Context::AddKeyFrame(SRS_TRACK& track, unsigned int frame) {
		switch (track.GetInterpolationType()) {
		case EInterpolationType::CONSTANT:
			switch (track.GetDataType()) {
			case ETrackDataType::FLOAT: AddKeyFrameEx<Key<float>>(track, frame); break;
			case ETrackDataType::INDEX: AddKeyFrameEx<Key<int>>(track, frame); break;
			case ETrackDataType::INT: AddKeyFrameEx<Key<int>>(track, frame); break;
			case ETrackDataType::BOOL: AddKeyFrameEx<Key<bool>>(track, frame); break;
			case ETrackDataType::COLOR: AddKeyFrameEx<Key<Color>>(track, frame); break;
			default: assert(false && "Invalid track flags"); break;
			}
			break;
		case EInterpolationType::LINEAR:
			switch (track.GetDataType()) {
			case ETrackDataType::FLOAT: AddKeyFrameEx<KeyLinear<float>>(track, frame); break;
			case ETrackDataType::INDEX: AddKeyFrameEx<KeyLinear<int>>(track, frame); break;
			case ETrackDataType::INT: AddKeyFrameEx<KeyLinear<int>>(track, frame); break;
			case ETrackDataType::BOOL: AddKeyFrameEx<KeyLinear<bool>>(track, frame); break;
			case ETrackDataType::COLOR: AddKeyFrameEx<KeyLinear<Color>>(track, frame); break;
			default: assert(false && "Invalid track flags"); break;
			}
			break;
		case EInterpolationType::HERMITE:
			switch (track.GetDataType()) {
			case ETrackDataType::FLOAT: AddKeyFrameEx<KeyHermite<float>>(track, frame); break;
			case ETrackDataType::INDEX: AddKeyFrameEx<KeyHermite<int>>(track, frame); break;
			case ETrackDataType::INT: AddKeyFrameEx<KeyHermite<int>>(track, frame); break;
			case ETrackDataType::BOOL: AddKeyFrameEx<KeyHermite<bool>>(track, frame); break;
			case ETrackDataType::COLOR: AddKeyFrameEx<KeyHermite<Color>>(track, frame); break;
			default: assert(false && "Invalid track flags"); break;
			}
			break;
		case EInterpolationType::INDIVIDUAL:
			switch (track.GetDataType()) {
			case ETrackDataType::FLOAT: AddKeyFrameEx<KeyIndividual<float>>(track, frame); break;
			case ETrackDataType::INDEX: AddKeyFrameEx<KeyIndividual<int>>(track, frame); break;
			case ETrackDataType::INT: AddKeyFrameEx<KeyIndividual<int>>(track, frame); break;
			case ETrackDataType::BOOL: AddKeyFrameEx<KeyIndividual<bool>>(track, frame); break;
			case ETrackDataType::COLOR: AddKeyFrameEx<KeyIndividual<Color>>(track, frame); break;
			default: assert(false && "Invalid track flags"); break;
			}
			break;
		default: assert(false && "Invalid track flags"); break;
		}
	}

	void Context::SetTrackStart(ucsl::resources::swif::v6::SRS_TRACK& track, unsigned int frame) {
		track.firstFrame = frame;
	}

	void Context::SetTrackEnd(ucsl::resources::swif::v6::SRS_TRACK& track, unsigned int frame) {
		track.lastFrame = frame;
	}

	void Context::MoveTrack(ucsl::resources::swif::v6::SRS_TRACK& track, int delta) {
		VisitKeyFrames(track, [&track, delta](auto* keys) {
			for (auto& key : std::span(keys, track.keyCount))
				key.frame = static_cast<unsigned int>(static_cast<int>(key.frame) + delta);
		});
	}

	SRS_SCENE* Context::FindScene(int id) const {
		for (auto& scene : std::span(project->scenes, project->sceneCount))
			if (scene.id == id)
				return &scene;

		return nullptr;
	}

	SRS_LAYER* Context::FindLayer(SRS_SCENE& scene, int id) const {
		for (auto& layer : std::span(scene.layers, scene.layerCount))
			if (layer.id == id)
				return &layer;

		return nullptr;
	}

	SRS_LAYER* Context::FindLayer(int id) const {
		for (auto& scene : std::span(project->scenes, project->sceneCount))
			if (auto* layer = FindLayer(scene, id))
				return layer;

		return nullptr;
	}

	SRS_CAMERA* Context::FindCamera(SRS_SCENE& scene, int id) const {
		for (auto& camera : std::span(scene.cameras, scene.cameraCount))
			if (camera.id == id)
				return &camera;

		return nullptr;
	}

	SRS_CAMERA* Context::FindCamera(int id) const {
		for (auto& scene : std::span(project->scenes, project->sceneCount))
			if (auto* camera = FindCamera(scene, id))
				return camera;

		return nullptr;
	}

	SRS_CASTNODE* Context::FindCast(SRS_LAYER& layer, int id) const {
		for (auto& cast : std::span(layer.casts, layer.castCount))
			if (cast.id == id)
				return &cast;

		return nullptr;
	}

	SRS_CASTNODE* Context::FindCast(SRS_SCENE& scene, int id) const {
		for (auto& layer : std::span(scene.layers, scene.layerCount))
			if (auto* cast = FindCast(layer, id))
				return cast;

		return nullptr;
	}

	SRS_CASTNODE* Context::FindCast(int id) const {
		for (auto& scene : std::span(project->scenes, project->sceneCount))
			if (auto* cast = FindCast(scene, id))
				return cast;

		return nullptr;
	}

	SRS_LAYER* Context::FindCastLayer(int castId) const {
		for (auto& scene : std::span(project->scenes, project->sceneCount))
			for (auto& layer : std::span(scene.layers, scene.layerCount))
				if (auto* found = FindCast(layer, castId))
					return &layer;
	}

	SRS_CASTNODE& Context::FindLastSibling(const SRS_LAYER& layer, SRS_CASTNODE& sibling) const {
		auto* res = &sibling;

		while (res->siblingIndex != -1)
			res = &layer.casts[res->siblingIndex];

		return *res;
	}

	SurfRide::Scene* Context::FindRuntimeScene(unsigned int id) const {
		if (gocSprite == nullptr)
			return nullptr;

		for (auto scene : gocSprite->GetProject()->GetScenes())
			if (scene->sceneData->id == id)
				return &*scene;

		return nullptr;
	}

	SurfRide::Layer* Context::FindRuntimeLayer(unsigned int id) const {
		if (gocSprite == nullptr)
			return nullptr;

		for (auto scene : gocSprite->GetProject()->GetScenes())
			if (auto* layer = scene->GetLayer(id))
				return layer;

		return nullptr;
	}

	SurfRide::Cast* Context::FindRuntimeCast(unsigned int id) const {
		auto* layer = FindCastLayer(id);

		if (!layer)
			return nullptr;

		auto* runtimeLayer = FindRuntimeLayer(layer->id);

		if (!runtimeLayer)
			return nullptr;

		return runtimeLayer->GetCast(id);
	}

	void Context::StartAnimationByIndex(const ucsl::resources::swif::v6::SRS_LAYER& layer, int animationIndex) {
		if (auto* runtimeLayer = FindRuntimeLayer(layer.id))
			runtimeLayer->StartAnimation(animationIndex);
	}

	float Context::GetAnimationFrame(const ucsl::resources::swif::v6::SRS_LAYER& layer) const {
		auto* runtimeLayer = FindRuntimeLayer(layer.id);

		return runtimeLayer ? runtimeLayer->currentFrame3 : 0.0f;
	}

	void Context::SetAnimationFrame(const ucsl::resources::swif::v6::SRS_LAYER& layer, float frame) {
		if (auto* runtimeLayer = FindRuntimeLayer(layer.id)) {
			runtimeLayer->currentFrame = frame;
			runtimeLayer->currentFrame2 = frame;
			runtimeLayer->currentFrame3 = frame;
		}
	}

	void Context::ApplyTransformChange(const ucsl::resources::swif::v6::SRS_CASTNODE& cast) {
		auto* layer = FindCastLayer(cast.id);
		auto* runtimeCast = FindRuntimeCast(cast.id);

		if (!runtimeCast)
			return;

		size_t castIndex = &cast - layer->casts;
		
		if (layer->Is3D()) {
			auto& transform = layer->transforms.transforms3d[castIndex];

#ifdef DEVTOOLS_TARGET_SDK_wars
			auto* castTransform = reinterpret_cast<SRS_TRS3D*>(runtimeCast->transformData);
			castTransform->position = transform.position;
			castTransform->rotation = transform.rotation;
			castTransform->scale = transform.scale;
			static_cast<SurfRide::Cast3D&>(cast).position = transform.position;
#else
			runtimeCast->transform->position = transform.position;
			runtimeCast->transform->rotation = transform.rotation;
			runtimeCast->transform->scale = transform.scale;
#endif
			runtimeCast->flags = cast.flags;
			runtimeCast->transform->materialColor = transform.materialColor;
			runtimeCast->transform->illuminationColor = transform.illuminationColor;
			runtimeCast->transform->display = transform.display;

			runtimeCast->transform->dirtyFlag.SetTransformAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
			runtimeCast->UpdateParentsAndThisTransformRecursively();
#endif
		}
		else {
			auto& transform = layer->transforms.transforms2d[castIndex];

#ifdef DEVTOOLS_TARGET_SDK_wars
			auto* castTransform = reinterpret_cast<SRS_TRS3D*>(runtimeCast->transformData);
			castTransform->position = { transform.position.x(), transform.position.y(), 0.0f };
			castTransform->rotation = { 0, 0, transform.rotation };
			castTransform->scale = { transform.scale.x(), transform.scale.y(), 0.0f };
			static_cast<SurfRide::Cast3D&>(cast).position = { transform.position.x(), transform.position.y(), 0.0f };
#else
			runtimeCast->transform->position = { transform.position.x(), transform.position.y(), 0.0f };
			runtimeCast->transform->rotation = { 0, 0, transform.rotation };
			runtimeCast->transform->scale = { transform.scale.x(), transform.scale.y(), 0.0f };
#endif
			runtimeCast->flags = cast.flags;
			runtimeCast->transform->materialColor = transform.materialColor;
			runtimeCast->transform->illuminationColor = transform.illuminationColor;
			runtimeCast->transform->display = transform.display;

			runtimeCast->transform->dirtyFlag.SetTransformAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
			runtimeCast->UpdateParentsAndThisTransformRecursively();
#endif
		}
	}

	void Context::ApplyImageCastChange(const ucsl::resources::swif::v6::SRS_CASTNODE& cast) {
		auto& imageCast = *cast.data.image;
		auto* runtimeCast = static_cast<SurfRide::ImageCast*>(FindRuntimeCast(cast.id));

		if (!runtimeCast)
			return;

		runtimeCast->size = imageCast.size;
#ifndef DEVTOOLS_TARGET_SDK_wars
		runtimeCast->vertexColorTopLeft = imageCast.vertexColorTopLeft;
		runtimeCast->vertexColorBottomLeft = imageCast.vertexColorBottomLeft;
		runtimeCast->vertexColorTopRight = imageCast.vertexColorTopRight;
		runtimeCast->vertexColorBottomRight = imageCast.vertexColorBottomRight;
#endif
		runtimeCast->cropIndex[0] = imageCast.cropIndex0;
		runtimeCast->cropIndex[1] = imageCast.cropIndex1;
		//runtimeCast->cropRectDirty[0] = true;
		//runtimeCast->cropRectDirty[1] = true;

#ifndef DEVTOOLS_TARGET_SDK_wars
		runtimeCast->blurEffect = nullptr;
		runtimeCast->reflectEffect = nullptr;
		runtimeCast->CreateEffectCast(imageCast.effectData.blur);
#endif

		runtimeCast->transform->dirtyFlag.SetCellAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
		runtimeCast->UpdateParentsAndThisTransformRecursively();
#endif
	}

	void Context::ApplyReferenceCastChange(const ucsl::resources::swif::v6::SRS_CASTNODE& cast) {
		auto& referenceCast = *cast.data.reference;
		auto* runtimeCast = static_cast<SurfRide::ReferenceCast*>(FindRuntimeCast(cast.id));

		if (!runtimeCast)
			return;

	}

	void Context::ApplySliceCastChange(const ucsl::resources::swif::v6::SRS_CASTNODE& cast) {
		auto& sliceCast = *cast.data.slice;
		auto* runtimeCast = static_cast<SurfRide::SliceCast*>(FindRuntimeCast(cast.id));

		if (!runtimeCast)
			return;

		runtimeCast->size = sliceCast.size;
#ifndef DEVTOOLS_TARGET_SDK_wars
		runtimeCast->vertexColorTopLeft = sliceCast.vertexColorTopLeft;
		runtimeCast->vertexColorBottomLeft = sliceCast.vertexColorBottomLeft;
		runtimeCast->vertexColorTopRight = sliceCast.vertexColorTopRight;
		runtimeCast->vertexColorBottomRight = sliceCast.vertexColorBottomRight;
#endif
#ifndef DEVTOOLS_TARGET_SDK_wars
		runtimeCast->blurEffect = nullptr;
		runtimeCast->reflectEffect = nullptr;
		runtimeCast->CreateEffectCast(sliceCast.effectData.blur);
#endif
		runtimeCast->transform->dirtyFlag.SetCellAll();
#ifdef DEVTOOLS_TARGET_SDK_wars
		runtimeCast->UpdateParentsAndThisTransformRecursively();
#endif
	}
}
