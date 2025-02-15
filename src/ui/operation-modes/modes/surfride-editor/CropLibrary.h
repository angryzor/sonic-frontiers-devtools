#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"
#include "Behaviors.h"
#include "texture-editor/TextureElement.h"
#include <imbrowser.h>

namespace ui::operation_modes::modes::surfride_editor {
	class CropLibrary : public Panel<Context> {
		using Panel::Panel;

		ImBrowser::ImBrowserContext* browserCtx{ ImBrowser::CreateContext() };
		csl::ut::MoveArray<texture_editor::TextureElement> currentPath{ GetAllocator() };

	public:
		virtual ~CropLibrary();
		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;

		void RenderTextureList(const texture_editor::TextureListRef& textureList);
		void RenderTexture(const texture_editor::TextureRef& texture);
		void RenderCrop(const texture_editor::CropRef& crop);
	};
}
