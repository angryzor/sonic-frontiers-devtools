#include "References.h"

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
	using namespace ucsl::resources::swif::swif_version;

	bool TextureListRef::operator==(const TextureListRef& other) const { return project == other.project && textureListIndex == other.textureListIndex; }
	SRS_TEXTURELIST& TextureListRef::GetTextureList() const { return project->projectData->textureLists[textureListIndex]; }
	SurfRide::TextureList* TextureListRef::GetTextureListInstance() const { return project->textureLists[textureListIndex]; }

	bool TextureRef::operator==(const TextureRef& other) const { return project == other.project && textureListIndex == other.textureListIndex && textureIndex == other.textureIndex; }
	SRS_TEXTURE& TextureRef::GetTexture() const { return project->projectData->textureLists[textureListIndex].textures[textureIndex]; }

	bool CropRef::operator==(const CropRef& other) const { return project == other.project && textureListIndex == other.textureListIndex && textureIndex == other.textureIndex && cropIndex == other.cropIndex; }
	SRS_CROP& CropRef::GetCrop() const { return project->projectData->textureLists[textureListIndex].textures[textureIndex].crops[cropIndex]; }
}
