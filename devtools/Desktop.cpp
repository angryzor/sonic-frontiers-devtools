#include "Pch.h"
#include "Desktop.h"
#include "ToolBar.h"
#include "icons.h"
#include "ResourceBrowser.h"

Desktop::Desktop(csl::fnd::IAllocator* allocator) : BaseObject{ allocator }
{
	resourceLoader = hh::fnd::ResourceLoader::Instantiate(allocator);
	resourceLoader->LoadPackfile("mods/angryzor_devtools/devtools.pac", 0);
	//iconTexture = (*rangerssdk::bootstrap::GetAddress(&hh::fnd::ResourceManager::instance))->GetResource<hh::gfnd::ResTexture>("devtools_icons.dds");
	//iconView = static_cast<
	//	hh::needle::ImplDX11::NeedleResourceContainer<hh::needle::Texture, hh::needle::ImplDX11::TextureDX11Impl<hh::needle::ImplDX11::SBufferTexture2D, hh::needle::ImplDX11::SViewTexture2D>>*
	//>(iconTexture->GetTexture())->QueryInterface<hh::needle::ImplDX11::SViewTexture2D>(DX11_VIEW_TEXTURE_2D)->view;
}

void Desktop::Render() {
	ToolBar::Render(this);
	operationMode.Render();
	for (auto& window : windows) {
		window->Render();
	}
}

void Desktop::OpenResourceBrowser() {
	StandaloneWindow* resourceBrowser = new (GetAllocator()) ResourceBrowser(GetAllocator());
	this->windows.push_back(hh::fnd::Reference<StandaloneWindow>{ resourceBrowser });
}