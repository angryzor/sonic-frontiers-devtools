#include "CropLibrary.h"
#include "Behaviors.h"
#include <ui/common/icons.h>

namespace ui::operation_modes::modes::surfride_editor {
	CropLibrary::~CropLibrary() {
		ImBrowser::DestroyContext(browserCtx);
	}

	void CropLibrary::RenderPanel()
	{
		auto& context = GetContext();
		if (context.project == nullptr)
			return;

		if (ImGui::Button("ROOT"))
			currentPath.clear();

		for (size_t i = 0; i < currentPath.size(); i++) {
			ImGui::SameLine();
			if (ImGui::Button(currentPath[i].GetName()))
				for (size_t j = currentPath.size() - 1; i < j; j--)
					currentPath.remove(j);
		}

		ImBrowser::Begin(browserCtx);
		if (ImBrowser::BeginBrowser("Crop browser", ImBrowser::ViewType::THUMBNAILS, nullptr, 0, nullptr, 0)) {
			if (currentPath.size() == 0) {
				for (size_t i = 0; i < context.project->textureListCount; i++) {
					auto& textureList = context.project->textureLists[i];
					auto icon = GetIconAsImage(IconId::RESOURCE_PACKFILE);
					ImBrowser::Image browserIcon{ .texId = icon.texId, .uv1 = icon.uv1, .uv2 = icon.uv2, .size = icon.size };

					ImGui::PushID(&textureList);
					if (ImBrowser::Item(&textureList, textureList.name, false, nullptr, nullptr, nullptr, 0, browserIcon, {}) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						currentPath.push_back(texture_editor::TextureListRef{ context.gocSprite->project, i });
					ImGui::PopID();
				}
			}
			else {
				auto& top = currentPath[currentPath.size() - 1];
				
				switch (top.type) {
				case texture_editor::TextureElement::Type::TEXTURELIST: {
					auto& textureList = top.textureList.GetTextureList();
					auto* textureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*top.textureList.GetTextureListInstance()->textureData);

					for (size_t i = 0; i < textureList.textureCount; i++) {
						auto& texture = textureList.textures[i];
						auto* needleTexture = &*textureData->textures[i];
						auto icon = GetIconAsImage(IconId::RESOURCE_PACKFILE);
						ImBrowser::Image browserIcon{ .texId = icon.texId, .uv1 = icon.uv1, .uv2 = icon.uv2, .size = icon.size };
						ImBrowser::Image browserThumbnail{ .texId = GetTextureIDFromMIRAGETexture(needleTexture), .uv1 = ImVec2(0.0f, 0.0f), .uv2 = ImVec2(1.0f, 1.0f), .size = ImVec2(texture.width, texture.height) };

						ImGui::PushID(&texture);
						if (ImBrowser::Item(&texture, texture.name, false, nullptr, nullptr, nullptr, 0, browserIcon, browserThumbnail) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
							currentPath.push_back(texture_editor::TextureRef{ context.gocSprite->project, top.textureList.textureListIndex, i });
						ImGui::PopID();
					}
					break;
				}
				case texture_editor::TextureElement::Type::TEXTURE: {
					auto& texture = top.texture.GetTexture();
					auto* textureData = static_cast<hh::ui::surfride::SurfRideTextureDataMIRAGE*>(&*top.texture.GetTextureListInstance()->textureData);
					auto* needleTexture = &*textureData->textures[top.texture.textureIndex];

					for (size_t i = 0; i < texture.cropCount; i++) {
						auto& crop = texture.crops[i];
						auto icon = GetIconAsImage(IconId::RESOURCE_PACKFILE);
						ImBrowser::Image browserIcon{ .texId = icon.texId, .uv1 = icon.uv1, .uv2 = icon.uv2, .size = icon.size };
						ImBrowser::Image browserThumbnail{
							.texId = GetTextureIDFromMIRAGETexture(needleTexture),
							.uv1 = ImVec2(crop.left, crop.top),
							.uv2 = ImVec2(crop.right, crop.bottom),
							.size = ImVec2((crop.right - crop.left) * texture.width, (crop.bottom - crop.top) * texture.height)
						};

						ImGui::PushID(&crop);

						char name[20];
						snprintf(name, 20, "%d", i);

						ImBrowser::Item(&texture, name, false, nullptr, nullptr, nullptr, 0, browserIcon, browserThumbnail);

						if (ImGui::IsItemActive() && ImGui::BeginDragDropSource()) {
							texture_editor::CropRef cropRef{ context.gocSprite->project, top.texture.textureListIndex, top.texture.textureIndex, i };
							ImGui::SetDragDropPayload("surfride:CropRef", &cropRef, sizeof(texture_editor::CropRef));
							ImGui::EndDragDropSource();
						}

						ImGui::PopID();
					}
					break;
				}
				}
			}
			ImBrowser::EndBrowser();
		}
	}

	PanelTraits CropLibrary::GetPanelTraits() const {
		return { "Crop library", ImVec2(0, 0), ImVec2(250, ImGui::GetMainViewport()->WorkSize.y - 600) };
	}
}
