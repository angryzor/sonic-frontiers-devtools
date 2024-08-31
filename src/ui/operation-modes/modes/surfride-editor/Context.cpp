#include "Context.h"
#include <resources/ManagedMemoryRegistry.h>
#include <resources/managed-memory/ManagedCArray.h>

namespace ui::operation_modes::modes::surfride_editor {
	using namespace SurfRide;

#ifdef DEVTOOLS_TARGET_SDK_wars
	csl::math::Matrix44 Context::GetSceneCameraMatrix(const Scene* scene)
	{
		auto resolution = GetScreenResolution();
		auto projectionMatrix = csl::math::CreatePerspectiveProjectionMatrix(
			static_cast<float>(scene->camera.camera.cameraData.fov) * 0.000095873802f,
			static_cast<float>(resolution.width) / static_cast<float>(resolution.height),
			scene->camera.camera.cameraData.nearPlane,
			scene->camera.camera.cameraData.farPlane
		);
		return projectionMatrix * scene->camera.viewMatrix;
	}
#else
	csl::math::Matrix44 Context::GetSceneCameraMatrix(const Scene* scene)
	{
		return scene->camera.projectionMatrix * scene->camera.viewMatrix;
	}
#endif

	csl::math::Matrix44 Context::GetFocusedSceneCameraMatrix() const
	{
		return GetSceneCameraMatrix(focusedScene);
	}

	csl::math::Matrix44 Context::GetFullCastTransform(const Cast* cast) {
		return GetSceneCameraMatrix(cast->layer->scene) * cast->transform->transformationMatrix;
	}

	void Context::CreateImageCast(Cast* sibling)
	{
		auto* layer = sibling->layer;
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(gocSprite->projectResource);

		auto* imageCast = new (&managedAllocator) SRS_IMAGECAST{};
		imageCast->flags = 0x00202000;
		imageCast->size = { 100.0f, 100.0f };
		imageCast->SetPivotType(EPivotType::CENTER_CENTER);
		imageCast->vertexColorTopLeft = { 255, 255, 255, 255 };
		imageCast->vertexColorBottomLeft = { 255, 255, 255, 255 };
		imageCast->vertexColorTopRight = { 255, 255, 255, 255 };
		imageCast->vertexColorBottomRight = { 255, 255, 255, 255 };

		resources::ManagedCArray<SRS_CASTNODE, int> casts{ gocSprite->projectResource, layer->layerData->casts, layer->layerData->castCount };

		auto prevSize = casts.size();

		SRS_CASTNODE newNode{};
		newNode.name = "img_newcast";
		newNode.id = static_cast<int>(mt());
		newNode.flags = 0x750;
		newNode.data = imageCast;
		newNode.SetType(SRS_CASTNODE::Type::IMAGE);

		casts.push_back(std::move(newNode));

		for (int i = 0; i < prevSize; i++)
			layer->casts[i]->castData = &layer->layerData->casts[i];

		auto* p{ sibling->castData };

		while (p->siblingIndex != -1)
			p = &layer->layerData->casts[p->siblingIndex];

		p->siblingIndex = static_cast<short>(prevSize);

		resources::ManagedCArray<SRS_TRS3D, int> transforms{ gocSprite->projectResource, layer->layerData->transforms.transforms3d, prevSize };
		transforms.emplace_back();
		transforms[transforms.size() - 1].materialColor = { 255, 255, 255, 255 };
		transforms[transforms.size() - 1].display = true;
	}

	void Context::AddImageCast(Layer* layer)
	{
		CreateImageCast(*layer->GetCasts().begin());
	}

	void Context::AddImageCast(Cast* parent) {
		CreateImageCast(*parent->GetChildren().begin());
	}
}
