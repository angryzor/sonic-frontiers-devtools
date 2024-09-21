#pragma once
#include <ui/operation-modes/Panel.h>
#include "Context.h"

namespace ui::operation_modes::modes::asm_editor::blend_tree_editor {
    class TextureElementInspector : public Panel<Context> {
    public:
        using Panel::Panel;

        virtual void RenderPanel() override;
        virtual PanelTraits GetPanelTraits() const override;
        void RenderTextureListInspector(const TextureListRef& textureListRef);
        void RenderTextureInspector(const TextureRef& textureRef);
        void RenderCropInspector(const CropRef& cropRef);
    };
}
