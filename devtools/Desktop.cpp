#include "Pch.h"
#include "Desktop.h"
#include "ToolBar.h"
#include "common/Icons.h"
#include "ResourceBrowser.h"

Desktop* Desktop::instance{};

Desktop::Desktop(csl::fnd::IAllocator* allocator) : BaseObject{ allocator }
{
	resourceLoader = hh::fnd::ResourceLoader::Create(allocator);
	resourceLoader->LoadPackfile("mods/angryzor_devtools/devtools.pac", 0);
	//iconTexture = (*rangerssdk::GetAddress(&hh::fnd::ResourceManager::instance))->GetResource<hh::gfnd::ResTexture>("devtools_icons.dds");
	//iconView = static_cast<
	//	hh::needle::ImplDX11::NeedleResourceContainer<hh::needle::Texture, hh::needle::ImplDX11::TextureDX11Impl<hh::needle::ImplDX11::SBufferTexture2D, hh::needle::ImplDX11::SViewTexture2D>>*
	//>(iconTexture->GetTexture())->QueryInterface<hh::needle::ImplDX11::SViewTexture2D>(DX11_VIEW_TEXTURE_2D)->view;
}

void Desktop::Render() {
	ToolBar::Render();
	operationMode.Render();

	csl::ut::MoveArray<StandaloneWindow*> windowsThatWantToClose{ hh::fnd::GetTempAllocator(hh::fnd::GetAllocatorSystem()) };

	for (auto& window : windows) {
		if (!window->Render())
			windowsThatWantToClose.push_back(window);
	}

	for (auto* window : windowsThatWantToClose) {
		RemoveStandaloneWindow(window);
	}
}

void Desktop::AddStandaloneWindow(StandaloneWindow* window) {
	this->windows.push_back(hh::fnd::Reference<StandaloneWindow>{ window });
}

void Desktop::RemoveStandaloneWindow(StandaloneWindow* window) {
	this->windows.remove(this->windows.find(window));
}
