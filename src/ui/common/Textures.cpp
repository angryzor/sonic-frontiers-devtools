#include "Textures.h"

ImTextureID GetTextureIDFromResTexture(const hh::gfnd::ResTexture* resTexture) {
	return resTexture->GetTexture()->QueryResource<hh::needle::ImplDX11::SViewTexture2D>(NEEDLE_RESOURCE_DX11_VIEW_TEXTURE_2D)->view;
}
