#include "FileSystemBrowser.h"

FileSystemBrowser::FileSystemBrowser(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator }, browser{ ImBrowser::CreateContext() } {
	SetTitle("File system browser");
}

FileSystemBrowser::~FileSystemBrowser() {
	ImBrowser::DestroyContext(browser);
}

void FileSystemBrowser::PreRender() {
	ImGui::SetNextWindowPos(ImVec2(250 + ImGui::GetStyle().ItemSpacing.x + ImGui::GetStyle().DisplaySafeAreaPadding.x + 2, ImGui::GetMainViewport()->WorkSize.y - 40), ImGuiCond_Once, ImVec2(0, 1));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->WorkSize.x - 250 - 800 - ImGui::GetStyle().ItemSpacing.x * 2, 300), ImGuiCond_Once);
	ImGui::SetNextWindowCollapsed(false, ImGuiCond_Once);
}

void FileSystemBrowser::RenderContents() {
	if (ImGui::BeginChild("Main area", {}, ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
		for (auto& fs : hh::fnd::FileSystem::GetInstance()->filesystems) {
			auto* protocol = fs.protocol.c_str();
			
			RenderDirectory(fs.filesystem, hh::fnd::InplaceTempUri<>{ protocol, strlen(protocol) });
		}
	}
	ImGui::EndChild();
}

void FileSystemBrowser::RenderFileSystem(hh::fnd::IFileSystem& fs) {
	
}

void FileSystemBrowser::RenderItem(hh::fnd::IFileSystem& fs, const hh::fnd::Uri& uri) {
	if (fs.IsFile(uri))
		RenderFile(fs, uri);
	else
		RenderDirectory(fs, uri);
}

void FileSystemBrowser::RenderDirectory(hh::fnd::IFileSystem& fs, const hh::fnd::Uri& uri) {
	auto* base = uri.GetUri();

	if (ImGui::TreeNode(base)) {
		hh::fnd::FileSet fileSet{ hh::fnd::MemoryRouter::GetTempAllocator() };

		fs.ListDirectory(uri, fileSet);

		for (auto& item : fileSet.items) {
			hh::fnd::InplaceTempUri<> childUri{ "" };
			childUri.SetPath(item.path);
			//csl::ut::String protocol{ hh::fnd::MemoryRouter::GetTempAllocator() };

			//uri.GetProtocol(protocol);
			//childUri.SetProtocol(protocol.c_str());

			RenderItem(fs, childUri);
		}

		ImGui::TreePop();
	}
}

void FileSystemBrowser::RenderFile(hh::fnd::IFileSystem& fs, const hh::fnd::Uri& uri) {
	ImGui::Text("%s", uri.GetUri());
}
