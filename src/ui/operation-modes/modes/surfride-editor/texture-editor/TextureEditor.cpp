#include "TextureEditor.h"
#include "TextureTree.h"
#include "TextureElementInspector.h"
#include "Behaviors.h"
#include <ui/common/Textures.h>

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
	TextureEditor::TextureEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host) : OperationMode{ allocator, host }
	{
		AddPanel<TextureTree>();
		AddPanel<TextureElementInspector>();
		AddBehavior<SelectionBehavior>();
		AddBehavior<ZoomBehavior>();
		AddBehavior<SelectionAabbBehavior>();
		AddBehavior<SelectionTransformationBehavior>();
		AddBehavior<ScreenSpaceManipulationBehavior>();
		AddBehavior<MousePickingBehavior>();
		AddBehavior<SelectionMousePickingBehavior>();
		AddBehavior<PlacementBehavior>();
	}

	void TextureEditor::RenderScene()
	{
		auto& context = GetContext();

		if (!context.focusedTexture.has_value())
			return;

		auto* textureList = context.focusedTexture->GetTextureListInstance();
		auto& texture = context.focusedTexture->GetTexture();

		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		auto& selection = selectionBehavior->GetSelection();

		auto zoom = GetBehavior<ZoomBehavior>()->zoomLevel;

		auto* textureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*textureList->textureData);

		if (textureData == nullptr)
			return;

		auto& needleTexture = textureData->textures[context.focusedTexture->textureIndex];

		if (needleTexture == nullptr)
			return;

		auto cursorPos = ImGui::GetCursorPos();
		auto pos = ImGui::GetCursorScreenPos();
		auto size = ImVec2(texture.width * zoom, texture.height * zoom);
		auto* dl = ImGui::GetWindowDrawList();

		ImGui::Image(GetTextureIDFromMIRAGETexture(needleTexture), size);

		ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 1.0f, 1.0f });
		for (size_t j = 0; j < texture.cropCount; j++) {
			auto& crop = texture.crops[j];

			auto topLeftOffset = ImVec2(crop.left * size.x, crop.top * size.y);
			auto bottomRightOffset = ImVec2(crop.right * size.x, crop.bottom * size.y);

			dl->AddRect(pos + topLeftOffset, pos + bottomRightOffset, selection.find(TextureElement{ CropRef{ context.gocSprite->GetProject(), context.focusedTexture->textureListIndex, context.focusedTexture->textureIndex, j}}) == -1 ? IM_COL32(255, 0, 255, 255) : IM_COL32(0, 255, 0, 255));

			ImGui::SetCursorPos(cursorPos + topLeftOffset + ImVec2(2, 0));
			ImGui::Text("%d", j);
		}
		ImGui::PopStyleColor();
	}
}
