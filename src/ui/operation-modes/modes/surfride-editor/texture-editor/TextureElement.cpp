#include "TextureElement.h"

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
	using namespace SurfRide;

	TextureElement::TextureElement(const TextureListRef& textureList) : type{ Type::TEXTURELIST }, textureList{ textureList } {}
	TextureElement::TextureElement(const TextureRef& texture) : type{ Type::TEXTURE }, texture{ texture } {}
	TextureElement::TextureElement(const CropRef& crop) : type{ Type::CROP }, crop{ crop } {}

	bool TextureElement::operator==(const TextureElement& other) const
	{
		if (type != other.type)
			return false;

		switch (type) {
		case Type::TEXTURELIST: return textureList == other.textureList;
		case Type::TEXTURE: return texture == other.texture;
		case Type::CROP: return crop == other.crop;
		default: return true;
		}
	}

	const char* TextureElement::GetName() const
	{
		switch (type) {
		case Type::TEXTURELIST: return textureList.GetTextureList().name;
#ifdef DEVTOOLS_TARGET_SDK_wars
		case Type::TEXTURE: return texture.GetTexture().filename;
#else
		case Type::TEXTURE: return texture.GetTexture().name;
#endif
		case Type::CROP: return "Crop";
		default: return nullptr;
		}
	}
}
