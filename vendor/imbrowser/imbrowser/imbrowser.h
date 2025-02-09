#pragma once
#include <imgui.h>

#ifdef small
#undef small
#endif

namespace ImBrowser {
	struct ImBrowserContext;

	enum class ViewType {
		THUMBNAILS,
		DETAILS,
		LIST,
		TILES,
		CONTENT,
	};

	struct Image {
		ImTextureID texId{};
		ImVec2 uv1{};
		ImVec2 uv2{};
		ImVec2 size{};
	};

	struct PropertyValue {
		const char* key{};
		const char* value{};
	};

	ImBrowserContext* CreateContext();
	void DestroyContext(ImBrowserContext* context);
	void Begin(ImBrowserContext* context);
	void End();
	bool BeginBrowser(const char* id, ViewType view, const char** rank1Properties, size_t rank1PropertyCount, const char** rank2Properties, size_t rank2PropertyCount);
	void EndBrowser();
	bool Item(void* id, const char* name, bool selected, bool* is_selected, bool* is_opened, PropertyValue* propertyValues, size_t propertyCount, Image icon, Image thumbnail);
}
