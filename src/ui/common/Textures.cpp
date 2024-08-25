#include "Textures.h"

ImTextureID GetTextureIDFromMIRAGETexture(hh::needle::Texture* texture)
{
	return texture->QueryResource<hh::needle::ImplDX11::SViewTexture2D>(NEEDLE_RESOURCE_DX11_VIEW_TEXTURE_2D)->view;
}

ImTextureID GetTextureIDFromResTexture(const hh::gfnd::ResTexture* resTexture) {
	auto* texture = resTexture->GetTexture();
	return texture == nullptr ? nullptr : GetTextureIDFromMIRAGETexture(texture);
}
