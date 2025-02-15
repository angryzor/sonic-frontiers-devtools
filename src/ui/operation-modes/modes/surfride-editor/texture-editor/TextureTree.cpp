#include "TextureTree.h"
#include "Behaviors.h"

namespace ui::operation_modes::modes::surfride_editor::texture_editor {
	void TextureTree::RenderPanel()
	{
		auto& context = GetContext();
		auto* project = context.gocSprite->GetProject();
		if (project == nullptr) {
			return;
		}

		//if (ImGui::BeginPopupContextItem()) {
		//	if (ImGui::Selectable("Add texture list"))
		//		;
		//	ImGui::EndPopup();
		//}

		for (size_t i = 0; i < project->textureListCount; i++)
			RenderTextureList({ project, i });
	}

	PanelTraits TextureTree::GetPanelTraits() const
	{
		return { "Texture tree", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 600) };
	}

	void TextureTree::RenderTextureList(const TextureListRef& textureListRef)
	{
		auto& context = GetContext();
		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		auto& selection = selectionBehavior->GetSelection();

		auto* textureList = textureListRef.GetTextureListInstance();

		const ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		ImGui::PushID(textureList);

		bool isOpen = ImGui::TreeNodeEx("TreeItem", nodeFlags | (selection.find(textureListRef) != -1 ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None), "%s", textureList->textureListData->name);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			context.focusedTexture = std::nullopt;
			selectionBehavior->Select(textureListRef);
		}

		if (ImGui::BeginDragDropTarget()) {
			if (auto* payload = ImGui::AcceptDragDropPayload("Resource")) {
				auto* res = *static_cast<hh::fnd::ManagedResource**>(payload->Data);
				const hh::fnd::ResourceTypeInfo* typeInfo = &res->GetClass();

				if (typeInfo == hh::gfnd::ResTexture::GetTypeInfo())
					context.AddTexture(textureListRef, static_cast<hh::gfnd::ResTexture*>(res));
			}
			ImGui::EndDragDropTarget();
		}

		if (isOpen) {
			for (size_t i = 0; i < textureList->textureListData->textureCount; i++)
				RenderTexture({ textureListRef.project, textureListRef.textureListIndex, i });

			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void TextureTree::RenderTexture(const TextureRef& textureRef)
	{
		auto& context = GetContext();
		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		auto& selection = selectionBehavior->GetSelection();

		auto& texture = textureRef.GetTexture();

		const ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		ImGui::PushID(&texture);
		
#ifdef DEVTOOLS_TARGET_SDK_wars
		const char* name = texture.filename;
#else
		const char* name = texture.name;
#endif
		bool isOpen = ImGui::TreeNodeEx("TreeItem", nodeFlags | (selection.find(textureRef) != -1 ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None), "%s", name);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			context.focusedTexture = textureRef;
			selectionBehavior->Select(textureRef);
		}

		if (ImGui::BeginDragDropTarget()) {
			if (auto* payload = ImGui::AcceptDragDropPayload("Resource")) {
				auto* res = *static_cast<hh::fnd::ManagedResource**>(payload->Data);
				const hh::fnd::ResourceTypeInfo* typeInfo = &res->GetClass();

				if (typeInfo == hh::gfnd::ResTexture::GetTypeInfo())
					context.ReplaceTexture(textureRef, static_cast<hh::gfnd::ResTexture*>(res));
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("surfride:TextureRef", &textureRef, sizeof(TextureRef));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Remove"))
				context.RemoveTexture(textureRef);
			ImGui::EndPopup();
		}

		if (isOpen) {
			for (size_t i = 0; i < texture.cropCount; i++)
				RenderCrop({ textureRef.project, textureRef.textureListIndex, textureRef.textureIndex, i });

			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void TextureTree::RenderCrop(const CropRef& cropRef)
	{
		auto& context = GetContext();
		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		auto& selection = selectionBehavior->GetSelection();

		auto& crop = cropRef.GetCrop();

		const ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		ImGui::PushID(&crop);

		ImGui::TreeNodeEx("TreeItem", nodeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | (selection.find(cropRef) != -1 ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None), "Crop %zd", cropRef.cropIndex);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			context.focusedTexture = { cropRef.project, cropRef.textureListIndex, cropRef.textureIndex };
			selectionBehavior->Select(cropRef);
		}

		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("surfride:CropRef", &cropRef, sizeof(CropRef));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Remove"))
				context.RemoveCrop(cropRef);
			ImGui::EndPopup();
		}

		ImGui::PopID();
	}
}
