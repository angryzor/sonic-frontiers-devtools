#pragma once

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
	struct TextureListRef {
		SurfRide::Project* project;
		size_t textureListIndex;

		bool operator==(const TextureListRef& other) const;
		ucsl::resources::swif::v6::SRS_TEXTURELIST& GetTextureList() const;
		SurfRide::TextureList* GetTextureListInstance() const;
	};

	struct TextureRef : TextureListRef {
		size_t textureIndex;

		bool operator==(const TextureRef& other) const;
		ucsl::resources::swif::v6::SRS_TEXTURE& GetTexture() const;
	};

	struct CropRef : TextureRef {
		size_t cropIndex;

		bool operator==(const CropRef& other) const;
		ucsl::resources::swif::v6::SRS_CROP& GetCrop() const;
	};
}
