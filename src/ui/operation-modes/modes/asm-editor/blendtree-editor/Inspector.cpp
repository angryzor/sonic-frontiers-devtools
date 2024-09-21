#include "TextureElementInspector.h"
#include "TextureElement.h"
#include "Behaviors.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/SurfRide.h>

namespace ui::operation_modes::modes::asm_editor::blend_tree_editor {
	using namespace SurfRide;

    void TextureElementInspector::RenderPanel() {
		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

		if (selection.size() == 0) {
			ImGui::Text("Select an item in the left pane.");
		}
		else if (selection.size() > 1) {
			ImGui::Text("Multiple items selected.");
		}
		else {
			auto& focusedElement = selection[0];

			switch (focusedElement.type) {
			case TextureElement::Type::TEXTURELIST:
				RenderTextureListInspector(focusedElement.textureList);
				break;
			case TextureElement::Type::TEXTURE:
				RenderTextureInspector(focusedElement.texture);
				break;
			case TextureElement::Type::CROP:
				RenderCropInspector(focusedElement.crop);
				break;
			}
		}
    }

    PanelTraits TextureElementInspector::GetPanelTraits() const {
		return { "Texture element inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImVec2(600, ImGui::GetMainViewport()->WorkSize.y - 140), ImVec2(1, 0) };
    }

    void TextureElementInspector::RenderTextureListInspector(const TextureListRef& textureListRef) {
		auto& textureList = textureListRef.GetTextureList();

		Editor("User data", GetContext().gocSprite->projectResource, textureList.userData);
    }

    void TextureElementInspector::RenderTextureInspector(const TextureRef& textureRef) {
		auto& texture = textureRef.GetTexture();

		Viewer("ID", texture.id);
#ifndef DEVTOOLS_TARGET_SDK_wars
		Viewer("Name", texture.name);
#endif
		Viewer("Filename", texture.filename);
		Editor("Width", texture.width);
		Editor("Height", texture.height);

		ImGui::Separator();
		Editor("User data", GetContext().gocSprite->projectResource, texture.userData);
    }

    void TextureElementInspector::RenderCropInspector(const CropRef& cropRef) {
		bool edited{};

		auto& crop = cropRef.GetCrop();

		edited |= Editor("Left", crop.left);
		edited |= Editor("Top", crop.top);
		edited |= Editor("Right", crop.right);
		edited |= Editor("Bottom", crop.bottom);

		if (edited)
			GetContext().RefreshAfterCropUpdate(cropRef);
    }
}
