#include "Pch.h"
#include "ResourceBrowser.h"
#include "resource-editors/ResReflectionEditor.h"
#include "resource-editors/ResObjectWorldEditor.h"
#include "common/Textures.h"
#include "common/Icons.h"
#include "serialization/resource-rfls/ResourceRfls.h"

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
		if (ImGui::MenuItem("Watch directory..."))
			ImGuiFileDialog::Instance()->OpenDialog("ResourceBrowserWatchDirectory", "Choose directory", nullptr, ".", ImGuiFileDialogFlags_Modal);

		ImGui::EndMenuBar();
	}

	if (ImGuiFileDialog::Instance()->Display("ResourceBrowserWatchDirectory", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk())
			WatchDirectory(ImGuiFileDialog::Instance()->GetFilePathName());

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

	RenderMainArea();
}

void ResourceBrowser::RenderMainArea() {
	//ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetContentRegionAvail().y));
	//if (ImGui::BeginChild("Resource types")) {
	if (currentPath.size() == 0) {
		auto* resourceManager = ResourceManager::GetInstance();

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
			else if (typeInfo == ResReflection<void>::GetTypeInfo()) {
				ResReflectionEditor::Create(Desktop::instance->GetAllocator(), static_cast<ResReflection<void>*>(resource));
			}
			//else if (typeInfo == hh::game::ResObjectWorld::GetTypeInfo()) {
			//	ResObjectWorldEditor::Create(Desktop::instance->GetAllocator(), static_cast<hh::game::ResObjectWorld*>(resource));
			//}
		}
		else
			selectedResource = resource;
	}

	ImGui::SetCursorPos(cursor);
	RenderPreview(resource, 100);

	ImGui::PopStyleVar();
	ImGui::EndGroup();

	if (ImGui::BeginItemTooltip()) {
		ImGui::PushTextWrapPos(300);
		ImGui::Text("Name: %s", resource->GetName());
		ImGui::Text("Type: %s", resource->GetClass().pName);
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

	ImGuiFileDialog::Instance()->OpenDialog("ResourceLoadFromFileDialog", "Choose File", extbuf, ".", ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite, resource);
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

	ImGuiFileDialog::Instance()->OpenDialog("ResourceExportDialog", "Choose File", extbuf, ".", ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite, resource);
}

void ResourceBrowser::RenderLoadDialog() {
	if (ImGuiFileDialog::Instance()->Display("ResourceLoadFromFileDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			ManagedResource* resource = static_cast<ManagedResource*>(ImGuiFileDialog::Instance()->GetUserDatas());
			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
			std::wstring wFilePath(filePath.begin(), filePath.end());

			ReloadResource(wFilePath.c_str(), resource);
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

const char* ResourceBrowser::GetExtensionByTypeInfo(const hh::fnd::ResourceTypeInfo* typeInfo)
{
	if (typeInfo == hh::game::ResObjectWorld::GetTypeInfo())
		return "gedit";
	else
		return ResourceTypeRegistry::GetInstance()->GetExtensionByTypeInfo(typeInfo);
}

const hh::fnd::ResourceTypeInfo* ResourceBrowser::GetTypeInfoByExtension(const char* extension)
{
	if (!strcmp(extension, "gedit"))
		return hh::game::ResObjectWorld::GetTypeInfo();
	else
		return ResourceTypeRegistry::GetInstance()->GetTypeInfoByExtension(extension);
}

void ResourceBrowser::RenderPreview(const hh::fnd::ManagedResource* resource, float size)
{
	auto cursor = ImGui::GetCursorPos();
	
	if (&resource->GetClass() == hh::gfnd::ResTexture::GetTypeInfo()) {
		auto* texture = static_cast<const hh::gfnd::ResTexture*>(resource);
		ImGui::Image(GetTextureIDFromResTexture(texture), ImVec2(size, size));
		ImGui::SetCursorPos(cursor + ImVec2(0, size - 32));
		RenderIcon(assetIcons, ResourceTypeToAssetIconId(&resource->GetClass()), ImVec2(32, 32));
	}
	else {
		ImGui::SetCursorPos(cursor + ImVec2((size - 64) / 2, (size - 64) / 2));
		RenderIcon(assetIcons, ResourceTypeToAssetIconId(&resource->GetClass()));
	}

	ImGui::SetCursorPos(cursor);
	ImGui::Dummy(ImVec2(size, size));
}

void ResourceBrowser::ReloadResource(const wchar_t* filePath, ManagedResource* resource) {
	HANDLE file = CreateFileW(filePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
		return;

	DWORD fileSize = GetFileSize(file, NULL);

	void* buffer = resource->GetAllocator()->Alloc(fileSize, 64);

	bool result = ReadFile(file, buffer, fileSize, NULL, NULL);
	CloseHandle(file);

	if (result) {
		if (&resource->GetClass() == hh::game::ResObjectWorld::GetTypeInfo()) {
			auto* res = static_cast<hh::game::ResObjectWorld*>(resource);
			void* oldResource = res->binaryData;

			resource->Load(buffer, fileSize);
			resource->GetAllocator()->Free(oldResource);

			auto* gameManager = hh::game::GameManager::GetInstance();
			if (gameManager) {
				auto* objWorld = gameManager->GetService<hh::game::ObjectWorld>();
				if (objWorld)
					for (auto* chunk : objWorld->GetWorldChunks())
						chunk->Restart(true);
			}
		}
		else {
			resource->Reload(buffer, fileSize);
			resource->GetAllocator()->Free(buffer);
		}
	}
	else {
		resource->GetAllocator()->Free(buffer);
	}
}

void ResourceBrowser::ExportResource(const wchar_t* filePath, ManagedResource* resource) {
	if (&resource->GetClass() == hh::game::ResObjectWorld::GetTypeInfo()) {
		ReflectionSerializer::SerializeToFile(filePath, static_cast<hh::game::ResObjectWorld*>(resource)->binaryData, ResourceRfls::resObjectWorld);
	}
}

filewatch::FileWatch<std::string>* ResourceBrowser::fileWatcher = nullptr;

void ResourceBrowser::WatchDirectory(const std::string& path) {
	auto* allocator = app::fnd::AppHeapManager::GetResidentAllocator();

	if (fileWatcher != nullptr) {
		fileWatcher->~FileWatch();
		allocator->Free(fileWatcher);
	}

	fileWatcher = new (allocator) filewatch::FileWatch<std::string>(path, [path](const std::string& subpath, const filewatch::Event changeType) {
		switch (changeType) {
		case filewatch::Event::added:
		case filewatch::Event::modified:
			{
				std::filesystem::path fspath{ path };
				fspath /= subpath;

				if (!std::filesystem::exists(fspath))
					return;

				std::wstring wfilename = fspath.filename();
				std::wstring wext = fspath.extension();

				std::string filename(wfilename.begin(), wfilename.end());
				size_t dotloc = filename.find('.');

				if (dotloc == std::string::npos)
					return;

				std::string fullExt = filename.substr(dotloc + 1);
				auto* resourceType = ResourceTypeRegistry::GetInstance()->GetTypeInfoByExtension(fullExt.c_str());

				if (resourceType == nullptr)
					return;

				auto* resource = ResourceManager::GetInstance()->GetResource(filename.c_str(), resourceType);

				if (resource == nullptr)
					return;

				ReloadResource(fspath.c_str(), resource);
			}
			break;
		}
	});
}
