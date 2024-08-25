#pragma once
#include "References.h"

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
	struct TextureElement {
		enum class Type {
			TEXTURELIST,
			TEXTURE,
			CROP,
		};

		Type type;
		union {
			TextureListRef textureList;
			TextureRef texture;
			CropRef crop;
		};

		TextureElement(const TextureListRef& textureList);
		TextureElement(const TextureRef& texture);
		TextureElement(const CropRef& crop);

		bool operator==(const TextureElement& other) const;
		const char* GetName() const;
	};
}
