#include "Context.h"
#include <resources/ManagedMemoryRegistry.h>
#include <resources/managed-memory/ManagedCArray.h>

using namespace SurfRide;

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
	void Context::AddTexture(const TextureListRef& textureListRef, hh::gfnd::ResTexture* textureResource) {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(gocSprite->projectResource);

		auto* textureList = textureListRef.GetTextureListInstance();
		auto* resourceTextureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*gocSprite->projectResource->textureDatas[textureListRef.textureListIndex]);
		auto* instanceTextureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*textureList->textureData);

#ifndef DEVTOOLS_TARGET_SDK_wars
		char* name = new (&managedAllocator) char[strlen(textureResource->GetName()) + 1];
		strcpy(name, textureResource->GetName());
#endif

		char* filename = new (&managedAllocator) char[strlen(textureResource->GetName()) + 1];
		strcpy(filename, textureResource->GetName());

		SRS_TEXTURE srsTexture{};
		srsTexture.width = static_cast<unsigned short>(textureResource->GetTexture()->width);
		srsTexture.height = static_cast<unsigned short>(textureResource->GetTexture()->height);
		srsTexture.id = mt();
#ifndef DEVTOOLS_TARGET_SDK_wars
		srsTexture.name = name;
#endif
		srsTexture.filename = filename;

		resources::ManagedCArray textures{ gocSprite->projectResource, textureList->textureListData->textures, textureList->textureListData->textureCount };

		textures.push_back(srsTexture);
		resourceTextureData->textures.push_back(textureResource->GetTexture());
		instanceTextureData->textures.push_back(textureResource->GetTexture());
	}

	void Context::ReplaceTexture(const TextureRef& textureRef, hh::gfnd::ResTexture* textureResource) {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(gocSprite->projectResource);

		auto* textureList = textureRef.GetTextureListInstance();
		auto& texture = textureRef.GetTexture();
		auto* resourceTextureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*gocSprite->projectResource->textureDatas[textureRef.textureListIndex]);
		auto* instanceTextureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*textureList->textureData);

		managedAllocator.Free(const_cast<char*>(texture.filename));

		char* filename = new (&managedAllocator) char[strlen(textureResource->GetName()) + 1];
		strcpy(filename, textureResource->GetName());

		texture.filename = filename;
		texture.width = static_cast<unsigned short>(textureResource->GetTexture()->width);
		texture.height = static_cast<unsigned short>(textureResource->GetTexture()->height);
		resourceTextureData->textures[textureRef.textureIndex] = textureResource->GetTexture();
		instanceTextureData->textures[textureRef.textureIndex] = textureResource->GetTexture();
	}

	void Context::RemoveTexture(const TextureRef& textureRef) {
		auto managedAllocator = resources::ManagedMemoryRegistry::instance->GetManagedAllocator(gocSprite->projectResource);

		auto* textureList = textureRef.GetTextureListInstance();
		auto& texture = textureRef.GetTexture();
		auto* resourceTextureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*gocSprite->projectResource->textureDatas[textureRef.textureListIndex]);
		auto* instanceTextureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*textureList->textureData);

#ifndef DEVTOOLS_TARGET_SDK_wars
		managedAllocator.Free(const_cast<char*>(texture.name));
#endif
		managedAllocator.Free(const_cast<char*>(texture.filename));
		managedAllocator.Free(const_cast<SRS_CROP*>(texture.crops));

		resources::ManagedCArray textures{ gocSprite->projectResource, textureList->textureListData->textures, textureList->textureListData->textureCount };

		textures.remove(static_cast<unsigned int>(textureRef.textureIndex));
		resourceTextureData->textures.remove(textureRef.textureIndex);
		instanceTextureData->textures.remove(textureRef.textureIndex);
	}

	CropRef Context::AddCrop(const TextureRef& textureRef, const SRS_CROP& crop)
	{
		auto& texture = textureRef.GetTexture();

		resources::ManagedCArray crops{ gocSprite->projectResource, texture.crops, texture.cropCount };

		crops.push_back(crop);

		return { textureRef.project, textureRef.textureListIndex, textureRef.textureIndex, crops.size() - 1 };
	}

	void Context::RemoveCrop(const CropRef& cropRef) {
		auto& texture = cropRef.GetTexture();

		resources::ManagedCArray crops{ gocSprite->projectResource, texture.crops, texture.cropCount };

		crops.remove(static_cast<unsigned int>(cropRef.cropIndex));
	}

	void Context::RefreshAfterTextureUpdate(const TextureRef& textureRef)
	{
		for (auto* scene : gocSprite->GetProject()->GetScenes())
			for (auto* layer : scene->GetLayers())
				for (auto* cast : layer->GetCasts())
					if (IsAffectedByTextureUpdate(textureRef, *cast->castData)) {
						cast->transform->dirtyFlag.SetCellAll();
						cast->transform->dirtyFlag.SetTransformAll();
					}
	}

	void Context::RefreshCastAfterTextureUpdate(const TextureRef& textureRef, SurfRide::Cast* cast)
	{
		if (IsAffectedByTextureUpdate(textureRef, *cast->castData)) {
			cast->transform->dirtyFlag.SetCellAll();
			cast->transform->dirtyFlag.SetTransformAll();
		}

		for (auto* child : cast->GetChildren())
			RefreshCastAfterTextureUpdate(textureRef, child);
	}

	bool Context::IsAffectedByTextureUpdate(const TextureRef& textureRef, SurfRide::SRS_CASTNODE& cast)
	{
		switch (cast.GetType()) {
		case SurfRide::SRS_CASTNODE::Type::IMAGE: {
			auto* data = static_cast<SurfRide::SRS_IMAGECAST*>(cast.data);

			for (size_t i = 0; i < data->cropRef0Count; i++)
				if (data->cropRefs0[i].textureListIndex == textureRef.textureListIndex && data->cropRefs0[i].textureIndex == textureRef.textureIndex)
					return true;

			for (size_t i = 0; i < data->cropRef1Count; i++)
				if (data->cropRefs1[i].textureListIndex == textureRef.textureListIndex && data->cropRefs1[i].textureIndex == textureRef.textureIndex)
					return true;
		}
		//case SurfRide::SRS_CASTNODE::Type::SLICE: {
		//	auto* data = static_cast<SurfRide::SRS_SLICECAST*>(cast.data);

		//	for (size_t i = 0; i < data->cropRef0Count; i++)
		//		if (data->cropRefs0[i].textureListIndex == cropRef.textureListIndex && data->cropRefs0[i].textureIndex == cropRef.textureIndex && data->cropRefs0[i].cropIndex == cropRef.cropIndex)
		//			return true;
		//}
		}

		return false;
	}

	void Context::RefreshAfterCropUpdate(const CropRef& cropRef)
	{
		for (auto* scene : gocSprite->GetProject()->GetScenes())
			for (auto* layer : scene->GetLayers())
				for (auto* cast : layer->GetCasts())
					RefreshCastAfterCropUpdate(cropRef, cast);
	}

	void Context::RefreshCastAfterCropUpdate(const CropRef& cropRef, SurfRide::Cast* cast)
	{
		if (IsAffectedByCropUpdate(cropRef, *cast->castData))
			cast->transform->dirtyFlag.SetCellCropUV();

		for (auto* child : cast->GetChildren())
			RefreshCastAfterCropUpdate(cropRef, child);
	}

	bool Context::IsAffectedByCropUpdate(const CropRef& cropRef, SurfRide::SRS_CASTNODE& cast)
	{
		switch (cast.GetType()) {
		case SurfRide::SRS_CASTNODE::Type::IMAGE: {
			auto* data = static_cast<SurfRide::SRS_IMAGECAST*>(cast.data);

			for (size_t i = 0; i < data->cropRef0Count; i++)
				if (data->cropRefs0[i].textureListIndex == cropRef.textureListIndex && data->cropRefs0[i].textureIndex == cropRef.textureIndex && data->cropRefs0[i].cropIndex == cropRef.cropIndex)
					return true;

			for (size_t i = 0; i < data->cropRef1Count; i++)
				if (data->cropRefs1[i].textureListIndex == cropRef.textureListIndex && data->cropRefs1[i].textureIndex == cropRef.textureIndex && data->cropRefs1[i].cropIndex == cropRef.cropIndex)
					return true;
		}
		//case SurfRide::SRS_CASTNODE::Type::SLICE: {
		//	auto* data = static_cast<SurfRide::SRS_SLICECAST*>(cast.data);

		//	for (size_t i = 0; i < data->cropRef0Count; i++)
		//		if (data->cropRefs0[i].textureListIndex == cropRef.textureListIndex && data->cropRefs0[i].textureIndex == cropRef.textureIndex && data->cropRefs0[i].cropIndex == cropRef.cropIndex)
		//			return true;
		//}
		}

		return false;
	}
}
