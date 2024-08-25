#pragma once
#include <utilities/CompatibleObject.h>
#include "TextureElement.h"
#include <optional>

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;
		hh::fnd::Reference<hh::ui::GOCSprite> gocSprite{};
		std::optional<TextureRef> focusedTexture{};

		void AddTexture(const TextureListRef& textureListRef, hh::gfnd::ResTexture* textureResource);
		void ReplaceTexture(const TextureRef& textureRef, hh::gfnd::ResTexture* textureResource);
		void RemoveTexture(const TextureRef& textureRef);
		CropRef AddCrop(const TextureRef& textureRef, const SurfRide::SRS_CROP& crop);
		void RemoveCrop(const CropRef& cropRef);

		void RefreshAfterTextureUpdate(const TextureRef& textureRef);
		void RefreshCastAfterTextureUpdate(const TextureRef& textureRef, SurfRide::Cast* cast);
		static bool IsAffectedByTextureUpdate(const TextureRef& textureRef, SurfRide::SRS_CASTNODE& cast);
		void RefreshAfterCropUpdate(const CropRef& cropRef);
		void RefreshCastAfterCropUpdate(const CropRef& cropRef, SurfRide::Cast* cast);
		static bool IsAffectedByCropUpdate(const CropRef& cropRef, SurfRide::SRS_CASTNODE& cast);
	};
}
