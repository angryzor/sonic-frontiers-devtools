#pragma once
#include <ui/common/StandaloneWindow.h>
#include <imbrowser.h>

class FileSystemBrowser : public StandaloneWindow {
    //csl::ut::MoveArray<hh::fnd::Reference<hh::fnd::Packfile>> currentPath{ GetAllocator() };
    ImBrowser::ImBrowserContext* browser{};
    ImBrowser::ViewType currentView{};

public:
    FileSystemBrowser(csl::fnd::IAllocator* allocator);
    virtual ~FileSystemBrowser();

    virtual void PreRender() override;
    virtual void RenderContents() override;

    void RenderFileSystem(hh::fnd::IFileSystem& fs);
    void RenderItem(hh::fnd::IFileSystem& fs, const hh::fnd::Uri& uri);
    void RenderDirectory(hh::fnd::IFileSystem& fs, const hh::fnd::Uri& uri);
    void RenderFile(hh::fnd::IFileSystem& fs, const hh::fnd::Uri& uri);
};
