#include "Pch.h"
#include "ResourceBrowser.h"
#include "editors/ResReflectionEditor.h"
//#include "editors/ResObjectWorldEditor.h"
#include "editors/ResMaterialEditor.h"
#include <resources/ReloadManager.h>
#include <ui/common/Textures.h>
#include <ui/common/Icons.h>
#include <ui/GlobalSettings.h>
#include <ui/common/viewers/Basic.h>
#include <utilities/ResourceTypes.h>
#include <ucsl-reflection/reflections/resources/object-world/v3.h>
#include <rip/binary/containers/binary-file/BinaryFile.h>

using namespace hh::fnd;

AssetIconId ResourceTypeToAssetIconId(const ResourceTypeInfo* typeInfo) {
	if (typeInfo == hh::snd::ResAtomConfig::GetTypeInfo()) return AssetIconId::ATOM_CONFIG;
	if (typeInfo == hh::snd::ResAtomCueSheet::GetTypeInfo()) return AssetIconId::ATOM_PLAYLIST;
	if (typeInfo == hh::gfx::ResMirageLight::GetTypeInfo()) return AssetIconId::LIGHTING;
	if (typeInfo == hh::gfx::ResMaterial::GetTypeInfo()) return AssetIconId::MATERIAL;
	if (typeInfo == hh::gfx::ResTerrainModel::GetTypeInfo()) return AssetIconId::TERRAIN_MODEL;
	if (typeInfo == hh::gfx::ResModel::GetTypeInfo()) return AssetIconId::MODEL;
	if (typeInfo == hh::gfnd::ResFragmentShader::GetTypeInfo()) return AssetIconId::FRAGMENT_SHADER;
	if (typeInfo == hh::gfnd::ResVertexShader::GetTypeInfo()) return AssetIconId::VERTEX_SHADER;
	if (typeInfo == hh::gfnd::ResTexture::GetTypeInfo()) return AssetIconId::TEXTURE;
	if (typeInfo == hh::font::ResBitmapFont::GetTypeInfo()) return AssetIconId::BITMAP_FONT;
	if (typeInfo == hh::font::ResScalableFontSet::GetTypeInfo()) return AssetIconId::SCALABLE_FONT;
	if (typeInfo == hh::fnd::ResReflection::GetTypeInfo()) return AssetIconId::REFLECTION;
	if (typeInfo == hh::fnd::Packfile::GetTypeInfo()) return AssetIconId::PACKFILE;
	if (typeInfo == hh::eff::ResEffect::GetTypeInfo()) return AssetIconId::PARTICLESYSTEM;
#ifdef DEVTOOLS_TARGET_SDK_rangers
	if (typeInfo == hh::gfnd::ResComputeShader::GetTypeInfo()) return AssetIconId::COMPUTE_SHADER;
	if (typeInfo == hh::ui::ResSurfRideProject::GetTypeInfo()) return AssetIconId::SURFRIDE_PROJECT;
	if (typeInfo == heur::resources::ResLevel::GetTypeInfo()) return AssetIconId::LEVEL;
	if (typeInfo == heur::resources::ResMasterLevel::GetTypeInfo()) return AssetIconId::MASTER_LEVEL;
	//if (typeInfo == Res::GetTypeInfo()) return AssetIconId::PACKFILE_LEVELS;
	if (typeInfo == heur::resources::ResPhysicalSkeleton::GetTypeInfo()) return AssetIconId::PHYSICS_BONE;
	//if (typeInfo == heur::resources::ResSoftBody::GetTypeInfo()) return AssetIconId::SOFTBODY;
	//if (typeInfo == heur::resources::ResHelperBone::GetTypeInfo()) return AssetIconId::HELPER_BONE;
	//if (typeInfo == heur::resources::ResOrcaData::GetTypeInfo()) return AssetIconId::ORCA;
	//if (typeInfo == Res::GetTypeInfo()) return AssetIconId::UI;
	if (typeInfo == heur::resources::ResAnimation::GetTypeInfo()) return AssetIconId::ANIMATION;
	if (typeInfo == heur::resources::ResSkeleton::GetTypeInfo()) return AssetIconId::SKELETON;
	if (typeInfo == heur::resources::ResDvScene::GetTypeInfo()) return AssetIconId::DVSCENE;
#endif
	return AssetIconId::MODEL;
}

ResourceBrowser::ResourceBrowser(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator } {
	SetTitle("Resource browser");
}

void ResourceBrowser::PreRender() {
	ImGui::SetNextWindowPos(ImVec2(250 + ImGui::GetStyle().ItemSpacing.x + ImGui::GetStyle().DisplaySafeAreaPadding.x + 2, ImGui::GetMainViewport()->WorkSize.y - 40), ImGuiCond_Once, ImVec2(0, 1));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->WorkSize.x - 250 - 800 - ImGui::GetStyle().ItemSpacing.x * 2, 300), ImGuiCond_Once);
	ImGui::SetNextWindowCollapsed(false, ImGuiCond_Once);
}

void ResourceBrowser::RenderContents() {
	if (ImGui::BeginMenuBar()) {
		//if (ImGui::MenuItem("Load file...")) {
		//	//ImGuiFileDialog::Instance()->OpenDialog("ResourceLoadFromFileDialog", "Choose File", ".*", ".", ImGuiFileDialogFlags_Modal);


		//	//auto* fileLoader = FileLoader::GetInstance();

		//	//fileLoader->UnkFunc8(foo);

		//	//InplaceTempUri<128> uri{ foo, sizeof(foo) };
		//	//ResourceLoader::Unk1 unk{ 1, "" };
		//	//Desktop::instance->resourceLoader->LoadResource(uri, hh::gfnd::ResTexture::GetTypeInfo(), 0, 1, unk);
		//}
		if (ImGui::MenuItem("Watch directory...")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal;
			ImGuiFileDialog::Instance()->OpenDialog("ResourceBrowserWatchDirectory", "Choose directory", nullptr, cfg);
		}

		ImGui::EndMenuBar();
	}

	if (ImGuiFileDialog::Instance()->Display("ResourceBrowserWatchDirectory", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk())
			ReloadManager::instance->WatchDirectory(ImGuiFileDialog::Instance()->GetCurrentPath());

		ImGuiFileDialog::Instance()->Close();
	}

	if (ImGui::Button("ROOT"))
		currentPath.clear();

	for (size_t i = 0; i < currentPath.size(); i++) {
		ImGui::SameLine();
		if (ImGui::Button(currentPath[i]->GetName()))
			for (size_t j = currentPath.size() - 1; i < j; j--)
				currentPath.remove(j);
	}

	if (ImGui::BeginChild("Main area"))
		RenderMainArea();
	ImGui::EndChild();
}

void ResourceBrowser::RenderMainArea() {
	if (currentPath.size() == 0) {
		auto* resourceManager = ResourceManager::GetInstance();

#ifdef DEVTOOLS_TARGET_SDK_miller
		RenderContainerContents(resourceManager->unpackedResourceContainer.uniqueResourceContainersByTypeInfo.GetValueOrFallback(hh::fnd::Packfile::GetTypeInfo(), nullptr));
#else
		for (auto* container : resourceManager->GetResourceContainers()) {
			RenderContainerContents(container);
		}
#endif
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
		if (ImGui::BeginPopupContextItem("Resource Context Menu")) {
			if (ImGui::MenuItem("Load from file")) {
				ShowLoadResourceDialog(resource);
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();
	}
}

void ResourceBrowser::RenderResource(ManagedResource* resource) {
	ImGui::BeginGroup();

	auto cursor = ImGui::GetCursorPos();

	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 1.0f));

	if (ImGui::Selectable(resource->GetName(), selectedResource == resource, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(100, 100 + ImGui::GetFontSize()))) {
		if (ImGui::IsMouseDoubleClicked(0)) {
			const ResourceTypeInfo* typeInfo = &resource->GetClass();

			if (typeInfo == Packfile::GetTypeInfo()) {
				selectedResource = nullptr;
				currentPath.push_back(static_cast<Packfile*>(resource));
			}
			else if (typeInfo == ResReflection::GetTypeInfo()) {
				ResReflectionEditor::Create(Desktop::instance->GetAllocator(), static_cast<ResReflection*>(resource));
			}
#ifndef DEVTOOLS_TARGET_SDK_wars
			else if (typeInfo == hh::gfx::ResMaterial::GetTypeInfo()) {
				ResMaterialEditor::Create(Desktop::instance->GetAllocator(), static_cast<hh::gfx::ResMaterial*>(resource));
			}
#endif
			//else if (typeInfo == hh::game::ResObjectWorld::GetTypeInfo()) {
			//	ResObjectWorldEditor::Create(Desktop::instance->GetAllocator(), static_cast<hh::game::ResObjectWorld*>(resource));
			//}
		}
		else
			selectedResource = resource;
	}

	if (ImGui::BeginDragDropSource()) {
		ImGui::SetDragDropPayload("Resource", &resource, sizeof(resource));
		ImGui::EndDragDropSource();
	}

	ImGui::SetCursorPos(cursor);
	RenderPreview(resource, 100);

	ImGui::PopStyleVar();
	ImGui::EndGroup();

	if (ImGui::BeginItemTooltip()) {
		ImGui::PushTextWrapPos(300);
		ImGui::Text("Name: %s", resource->GetName());
		ImGui::Text("Type: %s", resource->GetClass().pName);
		RenderDetails(resource);
		ImGui::PopTextWrapPos();
		ImGui::Separator();
		RenderPreview(resource, 300);
		ImGui::EndTooltip();
	}

	float lastButtonX = ImGui::GetItemRectMax().x;
	float nextButtonX = lastButtonX + ImGui::GetStyle().ItemSpacing.x + 100;

	if (nextButtonX < ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x)
		ImGui::SameLine();
}

void ResourceBrowser::ShowLoadResourceDialog(hh::fnd::ManagedResource* resource)
{
	char extbuf[20];
	const char* ext = GetExtensionByTypeInfo(&resource->GetClass());

	if (ext == nullptr)
		return;

	snprintf(extbuf, 20, ".%s", ext);

	IGFD::FileDialogConfig cfg{};
	cfg.path = GlobalSettings::defaultFileDialogDirectory;
	cfg.flags = ImGuiFileDialogFlags_Modal;
	cfg.userDatas = resource;
	ImGuiFileDialog::Instance()->OpenDialog("ResourceLoadFromFileDialog", "Choose File", extbuf, cfg);
}

void ResourceBrowser::RenderDialogs()
{
	RenderLoadDialog();
	RenderExportDialog();
}

void ResourceBrowser::ShowExportResourceDialog(hh::fnd::ManagedResource* resource)
{
	char extbuf[20];
	const char* ext = GetExtensionByTypeInfo(&resource->GetClass());

	if (ext == nullptr)
		return;

	snprintf(extbuf, 20, ".%s", ext);

	IGFD::FileDialogConfig cfg{};
	cfg.path = GlobalSettings::defaultFileDialogDirectory;
	cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
	cfg.userDatas = resource;
	ImGuiFileDialog::Instance()->OpenDialog("ResourceExportDialog", "Choose File", extbuf, cfg);
}

void ResourceBrowser::RenderLoadDialog() {
	if (ImGuiFileDialog::Instance()->Display("ResourceLoadFromFileDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			ManagedResource* resource = static_cast<ManagedResource*>(ImGuiFileDialog::Instance()->GetUserDatas());
			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
			std::wstring wFilePath(filePath.begin(), filePath.end());

			ReloadManager::instance->QueueReload(wFilePath, resource);
		}
		ImGuiFileDialog::Instance()->Close();
	}
}

void ResourceBrowser::RenderExportDialog() {
	if (ImGuiFileDialog::Instance()->Display("ResourceExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			ManagedResource* resource = static_cast<ManagedResource*>(ImGuiFileDialog::Instance()->GetUserDatas());
			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
			std::wstring wFilePath(filePath.begin(), filePath.end());

			ExportResource(wFilePath.c_str(), resource);
		}
		ImGuiFileDialog::Instance()->Close();
	}
}

void ResourceBrowser::RenderPreview(const hh::fnd::ManagedResource* resource, float size)
{
	auto cursor = ImGui::GetCursorPos();
	
	if (&resource->GetClass() == hh::gfnd::ResTexture::GetTypeInfo()) {
		auto* texture = static_cast<const hh::gfnd::ResTexture*>(resource);
		if (auto* textureId = GetTextureIDFromResTexture(texture)) {
			ImGui::Image(textureId, ImVec2(size, size));
			ImGui::SetCursorPos(cursor + ImVec2(0, size - 32));
			RenderIcon(assetIcons, ResourceTypeToAssetIconId(&resource->GetClass()), ImVec2(32, 32));
		}
		else {
			ImGui::SetCursorPos(cursor + ImVec2((size - 64) / 2, (size - 64) / 2));
			RenderIcon(assetIcons, ResourceTypeToAssetIconId(&resource->GetClass()));
		}
	}
	else {
		ImGui::SetCursorPos(cursor + ImVec2((size - 64) / 2, (size - 64) / 2));
		RenderIcon(assetIcons, ResourceTypeToAssetIconId(&resource->GetClass()));
	}

	ImGui::SetCursorPos(cursor);
	ImGui::Dummy(ImVec2(size, size));
}

void ResourceBrowser::ExportResource(const wchar_t* filePath, ManagedResource* resource) {
	if (&resource->GetClass() == hh::game::ResObjectWorld::GetTypeInfo()) {
		std::ofstream ofs{ filePath, std::ios::binary };
		rip::binary::binary_ostream bofs{ ofs };
		rip::binary::containers::binary_file::v2::BinaryFileSerializer serializer{ bofs };
		serializer.serialize<he2sdk::ucsl::GameInterface>(*(ucsl::resources::object_world::v3::ObjectWorldData<he2sdk::ucsl::GameInterface::AllocatorSystem>*)static_cast<hh::game::ResObjectWorld*>(resource)->binaryData);
	}
}

void ResourceBrowser::RenderDetails(const hh::fnd::ManagedResource* resource) {
	if (&resource->GetClass() == hh::gfnd::ResTexture::GetTypeInfo()) {
		if (auto* texture = static_cast<const hh::gfnd::ResTexture*>(resource)->GetTexture()) {
			ImGui::Text("Resolution: %d x %d x %d", texture->format.width, texture->format.height, texture->format.depth);
			Viewer("Array size", texture->format.arraySize);
			Viewer("Mip levels", texture->format.mipLevels);
			Viewer("Sample description index", texture->format.sampleDescIndex);
			Viewer("Type", static_cast<unsigned int>(texture->format.type));
			Viewer("Format", static_cast<unsigned int>(texture->format.format));
		}
	}
}
