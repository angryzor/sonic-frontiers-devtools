#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
	class TextureTree : public Panel<Context> {
	public:
		using Panel::Panel;

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;

		void RenderTextureList(const TextureListRef& textureList);
		void RenderTexture(const TextureRef& texture);
		void RenderCrop(const CropRef& crop);
	};
}
