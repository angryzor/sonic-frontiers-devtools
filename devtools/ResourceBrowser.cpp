#include "Pch.h"
#include "ResourceBrowser.h"
#include "resource-editors/ResReflectionEditor.h"
#include "Textures.h"
#include "icons.h"

using namespace hh::fnd;

namespace heur::resources {
	class ResAtomConfig { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResAtomCueSheet { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResMirageLight { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResMaterial { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResTerrainModel { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResModel { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResComputeShader { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResFragmentShader { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResVertexShader { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResLevel { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResMasterLevel { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResBitmapFont { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResScalableFontSet { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResEffect { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResPhysicalSkeleton { public: static const ResourceTypeInfo* GetTypeInfo(); };
	//class ResSoftBody { public: inline static const ResourceTypeInfo* GetTypeInfo(); };
	//class ResHelperBone { public: static const ResourceTypeInfo* GetTypeInfo(); };
	//class ResOrcaData { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResAnimation { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResSkeleton { public: static const ResourceTypeInfo* GetTypeInfo(); };
	class ResDvScene { public: static const ResourceTypeInfo* GetTypeInfo(); };
}

AssetIconId ResourceTypeToAssetIconId(const ResourceTypeInfo* typeInfo) {
	if (typeInfo == hh::ui::ResSurfRideProject::GetTypeInfo()) return AssetIconId::SURFRIDE_PROJECT;
	if (typeInfo == heur::resources::ResAtomConfig::GetTypeInfo()) return AssetIconId::ATOM_CONFIG;
	if (typeInfo == heur::resources::ResAtomCueSheet::GetTypeInfo()) return AssetIconId::ATOM_PLAYLIST;
	if (typeInfo == heur::resources::ResMirageLight::GetTypeInfo()) return AssetIconId::LIGHTING;
	if (typeInfo == heur::resources::ResMaterial::GetTypeInfo()) return AssetIconId::MATERIAL;
	if (typeInfo == heur::resources::ResTerrainModel::GetTypeInfo()) return AssetIconId::TERRAIN_MODEL;
	if (typeInfo == heur::resources::ResModel::GetTypeInfo()) return AssetIconId::MODEL;
	if (typeInfo == heur::resources::ResComputeShader::GetTypeInfo()) return AssetIconId::COMPUTE_SHADER;
	if (typeInfo == heur::resources::ResFragmentShader::GetTypeInfo()) return AssetIconId::FRAGMENT_SHADER;
	if (typeInfo == heur::resources::ResVertexShader::GetTypeInfo()) return AssetIconId::VERTEX_SHADER;
	if (typeInfo == hh::gfnd::ResTexture::GetTypeInfo()) return AssetIconId::TEXTURE;
	if (typeInfo == heur::resources::ResLevel::GetTypeInfo()) return AssetIconId::LEVEL;
	if (typeInfo == heur::resources::ResMasterLevel::GetTypeInfo()) return AssetIconId::MASTER_LEVEL;
	if (typeInfo == hh::font::ResBitmapFont::GetTypeInfo()) return AssetIconId::BITMAP_FONT;
	if (typeInfo == heur::resources::ResScalableFontSet::GetTypeInfo()) return AssetIconId::SCALABLE_FONT;
	if (typeInfo == hh::fnd::ResReflection<void>::GetTypeInfo()) return AssetIconId::REFLECTION;
	//if (typeInfo == Res::GetTypeInfo()) return AssetIconId::PACKFILE_LEVELS;
	if (typeInfo == hh::fnd::Packfile::GetTypeInfo()) return AssetIconId::PACKFILE;
	if (typeInfo == heur::resources::ResEffect::GetTypeInfo()) return AssetIconId::PARTICLESYSTEM;
	if (typeInfo == heur::resources::ResPhysicalSkeleton::GetTypeInfo()) return AssetIconId::PHYSICS_BONE;
	//if (typeInfo == heur::resources::ResSoftBody::GetTypeInfo()) return AssetIconId::SOFTBODY;
	//if (typeInfo == heur::resources::ResHelperBone::GetTypeInfo()) return AssetIconId::HELPER_BONE;
	//if (typeInfo == heur::resources::ResOrcaData::GetTypeInfo()) return AssetIconId::ORCA;
	//if (typeInfo == Res::GetTypeInfo()) return AssetIconId::UI;
	if (typeInfo == heur::resources::ResAnimation::GetTypeInfo()) return AssetIconId::ANIMATION;
	if (typeInfo == heur::resources::ResSkeleton::GetTypeInfo()) return AssetIconId::SKELETON;
	if (typeInfo == heur::resources::ResDvScene::GetTypeInfo()) return AssetIconId::DVSCENE;
	return AssetIconId::MODEL;
}

ResourceBrowser::ResourceBrowser(csl::fnd::IAllocator* allocator) : StandaloneWindow{allocator} {}

void ResourceBrowser::Render() {
	const ImGuiWindowFlags windowFlags
		= ImGuiWindowFlags_MenuBar;

	ImGui::SetNextWindowPos(ImVec2(250 + ImGui::GetStyle().ItemSpacing.x + ImGui::GetStyle().DisplaySafeAreaPadding.x + 2, ImGui::GetMainViewport()->WorkSize.y - 40), ImGuiCond_Once, ImVec2(0, 1));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->WorkSize.x - 250 - 800 - ImGui::GetStyle().ItemSpacing.x * 2, 300), ImGuiCond_Once);
	ImGui::SetNextWindowCollapsed(false, ImGuiCond_Once);
	if (ImGui::Begin("Resource browser", NULL, windowFlags)) {
		if (ImGui::Button("ROOT"))
			currentPath.clear();

		for (size_t i = 0; i < currentPath.size(); i++) {
			ImGui::SameLine();
			if (ImGui::Button(currentPath[i]->GetName()))
				for (size_t j = currentPath.size() - 1; i <= j; j--)
					currentPath.remove(j);
		}

		RenderMainArea();
	}
	ImGui::End();
}

void ResourceBrowser::RenderMainArea() {
	//ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetContentRegionAvail().y));
	//if (ImGui::BeginChild("Resource types")) {
	if (currentPath.size() == 0) {
		auto* resourceManager = *rangerssdk::bootstrap::GetAddress(&ResourceManager::instance);

		for (auto* container : resourceManager->GetResourceContainers()) {
			RenderContainerContents(container);
		}
	}
	else {
		for (auto* container : currentPath[currentPath.size() - 1]->GetResourceContainers()) {
			ImGui::PushID(container);
			RenderContainerContents(container);
			ImGui::PopID();
		}
	}
}

void ResourceBrowser::RenderContainerContents(const ResourceContainer* container) {
	auto resourceCount = container->GetNumResources();

	for (int i = 0; i < resourceCount; i++) {
		ManagedResource* resource = container->GetResourceByIndex(i);
		ImGui::PushID(resource);
		RenderResource(resource);
		ImGui::PopID();
	}
}

void ResourceBrowser::RenderResource(ManagedResource* resource) {
	ImGui::BeginGroup();

	auto cursor = ImGui::GetCursorPos();

	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 1.0f));
	if (ImGui::Selectable(resource->GetName(), selectedResource == resource, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(100, 100 + ImGui::GetFontSize()))) {
		if (ImGui::IsMouseDoubleClicked(0) && &resource->GetClass() == Packfile::GetTypeInfo()) {
			selectedResource = nullptr;
			currentPath.push_back(static_cast<Packfile*>(resource));
		}
		else
			selectedResource = resource;
	}

	if (&resource->GetClass() == hh::gfnd::ResTexture::GetTypeInfo()) {
		ImGui::SetCursorPos(cursor);
		auto texture = static_cast<hh::gfnd::ResTexture*>(resource);
		ImGui::Image(GetTextureIDFromResTexture(texture), ImVec2(100, 100));
		ImGui::SetCursorPos(cursor + ImVec2(0, 68));
		RenderIcon(assetIcons, ResourceTypeToAssetIconId(&resource->GetClass()), ImVec2(32, 32));
	}
	else {
		ImGui::SetCursorPos(cursor + ImVec2(18, 18));
		RenderIcon(assetIcons, ResourceTypeToAssetIconId(&resource->GetClass()));
	}

	ImGui::PopStyleVar();
	ImGui::EndGroup();

	float lastButtonX = ImGui::GetItemRectMax().x;
	float nextButtonX = lastButtonX + ImGui::GetStyle().ItemSpacing.x + 100;

	if (nextButtonX < ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x)
		ImGui::SameLine();
}

//void ResourceBrowser::RenderResources() {
//	if (ImGui::BeginChild("Resources")) {
//		if (selectedResourceContainer != nullptr) {
//			for (auto* resource : selectedResourceContainer->GetResources()) {
//				ImGui::Text();
//		}
//	}
//}
