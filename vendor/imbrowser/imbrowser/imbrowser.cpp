#include "imbrowser.h"
#include <imgui_internal.h>
#include <span>
#include <type_traits>

namespace ImBrowser {
	constexpr float smallThumbIconSize = 16.0f;
	constexpr float mediumThumbIconSize = 32.0f;
	constexpr float largeThumbIconSize = 64.0f;

	constexpr float previewThumbnailSize = 300.0f;

	struct ImBrowserContext {
		ImGuiTextBuffer textBuffer{};
		ImVector<const char*> rank1Properties{};
		ImVector<const char*> rank2Properties{};
		ViewType view{};
	};

	static ImBrowserContext* gCtx{};

	ImBrowserContext* CreateContext() {
		return IM_NEW(ImBrowserContext)();
	}

	void DestroyContext(ImBrowserContext* context) {
		IM_DELETE(context);
	}

	void Begin(ImBrowserContext* context) {
		gCtx = context;
	}

	void End() {
		gCtx = nullptr;
	}

	bool BeginBrowser(const char* id, ViewType view, const char** rank1Properties, size_t rank1PropertyCount, const char** rank2Properties, size_t rank2PropertyCount) {
		gCtx->view = view;
		gCtx->textBuffer.clear();
		gCtx->textBuffer.reserve(512);
		gCtx->rank1Properties.clear();
		gCtx->rank2Properties.clear();

		for (auto* prop : std::span(rank1Properties, rank1PropertyCount)) {
			const char* newStr = gCtx->textBuffer.c_str() + gCtx->textBuffer.size();
			gCtx->textBuffer.append(prop, prop + strlen(prop) + 1);
			gCtx->rank1Properties.push_back(newStr);
		}

		for (auto* prop : std::span(rank2Properties, rank2PropertyCount)) {
			const char* newStr = gCtx->textBuffer.c_str() + gCtx->textBuffer.size();
			gCtx->textBuffer.append(prop, prop + strlen(prop) + 1);
			gCtx->rank2Properties.push_back(newStr);
		}

		ImGui::PushID(id);

		if (gCtx->view == ViewType::LIST || gCtx->view == ViewType::TILES)
			ImGui::BeginGroup();

		if (gCtx->view == ViewType::DETAILS) {
			if (!ImGui::BeginTable("Allocated memory", 1 + gCtx->rank1Properties.size(), ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY)) {
				ImGui::PopID();
				return false;
			}

			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);

			for (auto* prop : gCtx->rank1Properties)
				ImGui::TableSetupColumn(prop);

			ImGui::TableHeadersRow();
		}

		return true;
	}

	void EndBrowser() {
		if (gCtx->view == ViewType::DETAILS)
			ImGui::EndTable();

		if (gCtx->view == ViewType::LIST || gCtx->view == ViewType::TILES)
			ImGui::EndGroup();

		ImGui::PopID();
	}

	const char* FindProp(const char* propKey, PropertyValue* propertyValues, size_t propertyCount) {
		for (size_t i = 0; i < propertyCount; i++)
			if (!strcmp(propertyValues[i].key, propKey))
				return propertyValues[i].value;

		return nullptr;
	}

	ImVec2 CalcFit(ImVec2 containerSize, ImVec2 contentSize) {
		auto ratio = ImVec2(containerSize / contentSize);
		auto fitRatio = std::min(ratio.x, ratio.y);

		return contentSize * fitRatio;
	}

	ImVec2 CalcFit(float squareSize, ImVec2 contentSize) {
		return CalcFit(ImVec2(squareSize, squareSize), contentSize);
	}

	void Center(ImVec2 containerSize, ImVec2 contentSize) {
		ImGui::SetCursorPos(ImGui::GetCursorPos() + (containerSize - contentSize) / 2);
	}

	void Center(float squareContainerSize, ImVec2 contentSize) {
		Center(ImVec2(squareContainerSize, squareContainerSize), contentSize);
	}

	//void SizedIcon(ImTextureID iconTexture, ImVec2 iconSize) {
	//	ImGui::Text("%s", icon);
	//}

	//void IconFit(ImTextureID iconTexture, ImVec2 iconSize, float size) {
	//	auto fit = CalcFit(mediumThumbIconSize, iconSize);
	//	Center(largeThumbIconSize, fit);
	//	SizedIcon(icon, fit);
	//}

	void ImageFit(Image thumbnail, float size) {
		auto fit = CalcFit(size, thumbnail.size);
		Center(size, fit);
		ImGui::Image(thumbnail.texId, fit, thumbnail.uv1, thumbnail.uv2);
	}

	void Thumbnail(float size, Image icon, Image thumbnail) {
		auto cursor = ImGui::GetCursorPos();

		if (thumbnail.texId) {
			ImageFit(thumbnail, size);

			if (size >= largeThumbIconSize) {
				ImGui::SetCursorPos(cursor + ImVec2(0.0f, size - mediumThumbIconSize));
				ImageFit(icon, mediumThumbIconSize);
			}
		}
		else {
			float iconSize = std::min(largeThumbIconSize, size);
			Center(size, ImVec2(iconSize, iconSize));
			ImageFit(icon, iconSize);
		}

		ImGui::SetCursorPos(cursor);
		ImGui::Dummy(ImVec2(size, size));
	}

	bool ThumbnailsItem(void* id, const char* name, bool selected, PropertyValue* propertyValues, size_t propertyCount, Image icon, Image thumbnail) {
		ImGui::PushID(id);
		ImGui::BeginGroup();

		auto cursor = ImGui::GetCursorPos();
		auto standardThumbnailSize = std::max(largeThumbIconSize, 100.0f * ImGui::GetFontSize() / 14.0f);

		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 1.0f));

		bool res = ImGui::Selectable(name, selected, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(standardThumbnailSize, standardThumbnailSize + ImGui::GetFontSize()));

		ImGui::SetCursorPos(cursor);
		Thumbnail(standardThumbnailSize, icon, thumbnail);

		ImGui::PopStyleVar();
		ImGui::EndGroup();
		ImGui::PopID();

		float lastButtonX = ImGui::GetItemRectMax().x;
		float nextButtonX = lastButtonX + ImGui::GetStyle().ItemSpacing.x + standardThumbnailSize;

		if (nextButtonX < ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x)
			ImGui::SameLine();

		return res;
	}

	bool DetailsItem(void* id, const char* name, bool selected, PropertyValue* propertyValues, size_t propertyCount, Image icon, Image thumbnail) {
		ImGui::PushID(id);
		ImGui::BeginGroup();
		ImGui::TableNextRow(0);
		ImGui::TableNextColumn();

		auto beforeCursor = ImGui::GetCursorPos();

		//bool isOpen = ImGui::TreeNodeEx(id, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAllColumns | (selected ? ImGuiTreeNodeFlags_Selected : 0), "%s", name);
		bool res = ImGui::Selectable("", selected, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.0f, smallThumbIconSize));

		auto cursor = ImGui::GetCursorPos();

		ImGui::SetCursorPos(beforeCursor);
		Thumbnail(smallThumbIconSize, icon, thumbnail);
		ImGui::SameLine();
		ImGui::Text("%s", name);
		ImGui::SameLine();

		for (auto* prop : gCtx->rank1Properties) {
			ImGui::TableNextColumn();

			if (auto* propVal = FindProp(prop, propertyValues, propertyCount))
				ImGui::Text("%s", propVal);
		}
		ImGui::EndGroup();
		ImGui::PopID();

		return res;
	}

	bool ListItem(void* id, const char* name, bool selected, PropertyValue* propertyValues, size_t propertyCount, Image icon, Image thumbnail) {
		ImVec2 cSize = ImGui::GetContentRegionAvail();

		if (cSize.y < ImGui::GetFontSize()) {
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();
		}

		auto textSize = ImGui::CalcTextSize(name);

		ImGui::PushID(id);
		ImGui::BeginGroup();

		auto cursor = ImGui::GetCursorPos();

		bool res = ImGui::Selectable("", selected, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(textSize.x + smallThumbIconSize, smallThumbIconSize));

		ImGui::SetCursorPos(cursor);
		Thumbnail(ImGui::GetFontSize(), icon, thumbnail);
		ImGui::SameLine();
		ImGui::Text("%s", name);
		ImGui::EndGroup();
		ImGui::PopID();

		return res;
	}

	bool TilesItem(void* id, const char* name, bool selected, PropertyValue* propertyValues, size_t propertyCount, Image icon, Image thumbnail) {
		auto height = std::max((1 + gCtx->rank1Properties.size()) * ImGui::GetFontSize(), largeThumbIconSize);
		ImVec2 cSize = ImGui::GetContentRegionAvail();

		if (cSize.y < height) {
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();
		}

		ImGui::PushID(id);
		ImGui::BeginGroup();

		auto cursor = ImGui::GetCursorPos();

		ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(1.0f, 0.5f));

		bool res = ImGui::Selectable("", selected, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(height + 200.0f, height));

		ImGui::SetCursorPos(cursor);
		Thumbnail(height, icon, thumbnail);
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (height - ImGui::GetFontSize()) / 2.0f);
		ImGui::Text("%s", name);

		ImGui::PopStyleVar();
		ImGui::EndGroup();
		ImGui::PopID();

		return res;
	}

	void Tooltip(void* id, const char* name, PropertyValue* propertyValues, size_t propertyCount, Image icon, Image thumbnail) {
		if (ImGui::BeginItemTooltip()) {
			ImGui::PushTextWrapPos(previewThumbnailSize);

			for (size_t i = 0; i < propertyCount; i++)
				ImGui::Text("%s: %s", propertyValues[i].key, propertyValues[i].value);

			ImGui::PopTextWrapPos();
			ImGui::Separator();
			Thumbnail(previewThumbnailSize, icon, thumbnail);
			ImGui::EndTooltip();
		}
	}

	bool ItemCore(void* id, const char* name, bool selected, PropertyValue* propertyValues, size_t propertyCount, Image icon, Image thumbnail) {
		switch (gCtx->view) {
		case ViewType::THUMBNAILS: return ThumbnailsItem(id, name, selected, propertyValues, propertyCount, icon, thumbnail);
		case ViewType::DETAILS: return DetailsItem(id, name, selected, propertyValues, propertyCount, icon, thumbnail);
		case ViewType::LIST: return ListItem(id, name, selected, propertyValues, propertyCount, icon, thumbnail);
		case ViewType::TILES: return TilesItem(id, name, selected, propertyValues, propertyCount, icon, thumbnail);
		//case ViewType::CONTENT: return ContentItem(id, name, selected, propertyValues, propertyCount, iconTexture, thumbnail);
		}
	}

	bool Item(void* id, const char* name, bool selected, bool* is_selected, bool* is_opened, PropertyValue* propertyValues, size_t propertyCount, Image icon, Image thumbnail) {
		bool clicked = ItemCore(id, name, selected, propertyValues, propertyCount, icon, thumbnail);

		if (clicked) {
			if (is_selected)
				*is_selected = true;

			if (is_opened && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				*is_opened = true;
		}

		Tooltip(id, name, propertyValues, propertyCount, icon, thumbnail);

		return clicked;
	}
}
