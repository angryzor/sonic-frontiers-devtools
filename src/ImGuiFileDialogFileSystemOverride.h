#pragma once
#include <filesystem>
#include <iostream>

// Builtin std::filesystem filesystem copied here because you can only select 1 in ImGuiFileDialog.
static std::filesystem::path stringToPath(const std::string& str)
{
#ifdef _IGFD_WIN_
    return std::filesystem::path(IGFD::Utils::UTF8Decode(str));
#else
    return std::filesystem::path(str);
#endif
}

static std::string pathToString(const std::filesystem::path& path)
{
#ifdef _IGFD_WIN_
    return IGFD::Utils::UTF8Encode(path.wstring());
#else
    return path.string();
#endif
}

class FileSystemStd : public IGFD::IFileSystem {
public:
    const char* GetRootPrefix() const override { return "\\"; }
    const char* GetPathSeparator() const override { return "\\"; }
    bool IsDirectoryCanBeOpened(const std::string& vName) override {
        bool bExists = false;
        if (!vName.empty()) {
            namespace fs = std::filesystem;
            auto pathName = stringToPath(vName);
            try {
                // interesting, in the case of a protected dir or for any reason the dir cant be opened
                // this func will work but will say nothing more . not like the dirent version
                bExists = fs::is_directory(pathName);
                // test if can be opened, this function can thrown an exception if there is an issue with this dir
                // here, the dir_iter is need else not exception is thrown..
                const auto dir_iter = fs::directory_iterator(pathName);
                (void)dir_iter;  // for avoid unused warnings
            } catch (const std::exception& /*ex*/) {
                // fail so this dir cant be opened
                bExists = false;
            }
        }
        return bExists;  // this is not a directory!
    }
    bool IsDirectoryExist(const std::string& vName) override {
        if (!vName.empty()) {
            namespace fs = std::filesystem;
            return fs::is_directory(stringToPath(vName));
        }
        return false;  // this is not a directory!
    }
    bool IsFileExist(const std::string& vName) override {
        namespace fs = std::filesystem;
        return fs::is_regular_file(stringToPath(vName));
    }
    bool CreateDirectoryIfNotExist(const std::string& vName) override {
        if (vName.empty()) return false;
        if (IsDirectoryExist(vName)) return true;

#if defined(__EMSCRIPTEN__)
        std::string str = std::string("FS.mkdir('") + vName + "');";
        emscripten_run_script(str.c_str());
        bool res = true;
#else
        namespace fs = std::filesystem;
        bool res = fs::create_directory(stringToPath(vName));
#endif  // _IGFD_WIN_
        if (!res) {
            std::cout << "Error creating directory " << vName << std::endl;
        }
        return res;
    }

    std::vector<IGFD::PathDisplayedName> GetDevicesList() override {
        std::vector<IGFD::PathDisplayedName> res;
#ifdef _IGFD_WIN_
        const DWORD mydrives = 2048;
        char lpBuffer[2048];
#define mini(a, b) (((a) < (b)) ? (a) : (b))
        const DWORD countChars = mini(GetLogicalDriveStringsA(mydrives, lpBuffer), 2047);
#undef mini
        if (countChars > 0U && countChars < 2049U) {
            std::string var = std::string(lpBuffer, (size_t)countChars);
            IGFD::Utils::ReplaceString(var, "\\", "");
            auto arr = IGFD::Utils::SplitStringToVector(var, '\0', false);
            wchar_t szVolumeName[2048];
            IGFD::PathDisplayedName path_name;
            for (auto& a : arr) {
                path_name.first = a;
                path_name.second.clear();
                std::wstring wpath = IGFD::Utils::UTF8Decode(a);
                if (GetVolumeInformationW(wpath.c_str(), szVolumeName, 2048, nullptr, nullptr, nullptr, nullptr, 0)) {
                    path_name.second = IGFD::Utils::UTF8Encode(szVolumeName);
                }
                res.push_back(path_name);
            }
        }
#endif  // _IGFD_WIN_
        return res;
    }

    IGFD::Utils::PathStruct ParsePathFileName(const std::string& vPathFileName) override {
        // https://github.com/aiekick/ImGuiFileDialog/issues/54
        namespace fs = std::filesystem;
        IGFD::Utils::PathStruct res;
        if (vPathFileName.empty()) return res;
        auto fsPath = stringToPath(vPathFileName);
        if (fs::is_directory(fsPath)) {
            res.name = "";
            res.path = pathToString(fsPath);
            res.isOk = true;
        } else if (fs::is_regular_file(fsPath)) {
            res.name = pathToString(fsPath.filename());
            res.path = pathToString(fsPath.parent_path());
            res.isOk = true;
        }
        return res;
    }

    std::vector<IGFD::FileInfos> ScanDirectory(const std::string& vPath) override {
        std::vector<IGFD::FileInfos> res;
        try {
            namespace fs = std::filesystem;
            auto fspath = stringToPath(vPath);
            const auto dir_iter   = fs::directory_iterator(fspath);
            IGFD::FileType fstype = IGFD::FileType(IGFD::FileType::ContentType::Directory, fs::is_symlink(fs::status(fspath)));
            {
                IGFD::FileInfos file_two_dot;
                file_two_dot.filePath    = vPath;
                file_two_dot.fileNameExt = "..";
                file_two_dot.fileType    = fstype;
                res.push_back(file_two_dot);
            }
            for (const auto& file : dir_iter) {
                try {
                    IGFD::FileType fileType;
                    if (file.is_symlink()) {
                        fileType.SetSymLink(file.is_symlink());
                        fileType.SetContent(IGFD::FileType::ContentType::LinkToUnknown);
                    }
                    if (file.is_directory()) {
                        fileType.SetContent(IGFD::FileType::ContentType::Directory);
                    }  // directory or symlink to directory
                    else if (file.is_regular_file()) {
                        fileType.SetContent(IGFD::FileType::ContentType::File);
                    }
                    if (fileType.isValid()) {
                        auto fileNameExt = pathToString(file.path().filename());
                        {
                            IGFD::FileInfos _file;
                            _file.filePath    = vPath;
                            _file.fileNameExt = fileNameExt;
                            _file.fileType    = fileType;
                            res.push_back(_file);
                        }
                    }
                } catch (const std::exception& ex) {
                    std::cout << "IGFD : " << ex.what() << std::endl;
                }
            }
        } catch (const std::exception& ex) {
            std::cout << "IGFD : " << ex.what() << std::endl;
        }
        return res;
    }
    bool IsDirectory(const std::string& vFilePathName) override {
        namespace fs = std::filesystem;
        return fs::is_directory(stringToPath(vFilePathName));
    }
};

class FileSystemHE2 : public IGFD::IFileSystem {
public:
    const char* GetRootPrefix() const override { return "\\"; }
    const char* GetPathSeparator() const override { return "\\"; }
    bool IsDirectoryCanBeOpened(const std::string& vName) override {
        return !vName.empty() && hh::fnd::FileSystem::GetInstance()->IsDirectoryByPath(vName.c_str());
    }
    bool IsDirectoryExist(const std::string& vName) override {
        return !vName.empty() && hh::fnd::FileSystem::GetInstance()->IsDirectoryByPath(vName.c_str());
    }
    bool IsFileExist(const std::string& vName) override {
        return !vName.empty() && hh::fnd::FileSystem::GetInstance()->IsFileByPath(vName.c_str());
    }
    bool CreateDirectoryIfNotExist(const std::string& vName) override {
        if (vName.empty()) return false;
        if (IsDirectoryExist(vName)) return true;
        return hh::fnd::FileSystem::GetInstance()->CreateDirectoryByPath(vName.c_str());
    }

    std::vector<IGFD::PathDisplayedName> GetDevicesList() override {
        std::vector<IGFD::PathDisplayedName> res{};

        for (auto& filesystem : hh::fnd::FileSystem::GetInstance()->filesystems)
            res.push_back({ filesystem.protocol.c_str(), {} });

        return res;
    }

    IGFD::Utils::PathStruct ParsePathFileName(const std::string& vPathFileName) override {
        auto* fs = hh::fnd::FileSystem::GetInstance();
        IGFD::Utils::PathStruct res{};
        if (vPathFileName.empty()) return res;
        if (fs->IsDirectoryByPath(vPathFileName.c_str())) {
            res.name = "";
            res.path = vPathFileName;
            res.isOk = true;
        } else if (fs->IsFileByPath(vPathFileName.c_str())) {
            auto loc = vPathFileName.rfind('\\');

            if (loc == std::string::npos) {
                res.name = vPathFileName.substr(loc + 1);
                res.path = vPathFileName.substr(0, loc);
            }
            else {
                res.name = vPathFileName;
                res.path = "";
            }
            res.isOk = true;
        }
        return res;
    }

    std::vector<IGFD::FileInfos> ScanDirectory(const std::string& vPath) override {
        std::vector<IGFD::FileInfos> res;

        {
            IGFD::FileInfos file_two_dot;
            file_two_dot.filePath = vPath;
            file_two_dot.fileNameExt = "..";
            file_two_dot.fileType = IGFD::FileType(IGFD::FileType::ContentType::Directory, false);
            res.push_back(file_two_dot);
        }

        csl::ut::String protocol{ hh::fnd::MemoryRouter::GetTempAllocator() };
        csl::ut::String path{ hh::fnd::MemoryRouter::GetTempAllocator() };
        hh::fnd::InplaceTempUri<>{ vPath.c_str(), vPath.size() }.GetProtocol(protocol);

        auto* fs = hh::fnd::FileSystem::GetInstance();
        hh::fnd::FileSet fileset{ hh::fnd::MemoryRouter::GetTempAllocator() };
        fs->ListDirectoryByPath(vPath.c_str(), fileset);

        for (auto& item : fileset.items) {
            IGFD::FileType fileType;
            hh::fnd::InplaceTempUri<> uri{ "" };
            uri.SetProtocol(protocol.c_str());
            uri.SetPath(item.path);

            if (fs->IsDirectory(uri))
                fileType.SetContent(IGFD::FileType::ContentType::Directory);
            else if (fs->IsFile(uri))
                fileType.SetContent(IGFD::FileType::ContentType::File);
            
            if (fileType.isValid()) {
                auto slashPos = strrchr(item.path, '\\');
                IGFD::FileInfos _file;
                _file.filePath    = vPath;
                _file.fileNameExt = !slashPos ? item.path : slashPos + 1;
                _file.fileType    = fileType;
                res.push_back(_file);
            }
        }

        return res;
    }

    bool IsDirectory(const std::string& vFilePathName) override {
        return !vFilePathName.empty() && hh::fnd::FileSystem::GetInstance()->IsDirectoryByPath(vFilePathName.c_str());
    }
};

class FileSystemCombined : public IGFD::IFileSystem {
    FileSystemStd stdFS;
    FileSystemHE2 he2FS;

    bool IsDirectoryCanBeOpened(const std::string& vName) override {
        return hh::fnd::FileSystem::GetInstance()->GetFileSystem(hh::fnd::InplaceTempUri<>{ vName.c_str(), vName.size() }) ? he2FS.IsDirectoryCanBeOpened(vName) : stdFS.IsDirectoryCanBeOpened(vName);
    }

    bool IsDirectoryExist(const std::string& vName) override {
        return hh::fnd::FileSystem::GetInstance()->GetFileSystem(hh::fnd::InplaceTempUri<>{ vName.c_str(), vName.size() }) ? he2FS.IsDirectoryExist(vName) : stdFS.IsDirectoryExist(vName);
    }

    bool IsFileExist(const std::string& vName) override {
        return hh::fnd::FileSystem::GetInstance()->GetFileSystem(hh::fnd::InplaceTempUri<>{ vName.c_str(), vName.size() }) ? he2FS.IsFileExist(vName) : stdFS.IsFileExist(vName);
    }

    bool CreateDirectoryIfNotExist(const std::string& vName) override {
        return hh::fnd::FileSystem::GetInstance()->GetFileSystem(hh::fnd::InplaceTempUri<>{ vName.c_str(), vName.size() }) ? he2FS.CreateDirectoryIfNotExist(vName) : stdFS.CreateDirectoryIfNotExist(vName);
    }

    std::vector<IGFD::PathDisplayedName> GetDevicesList() override {
        auto he2Res = he2FS.GetDevicesList();
        auto res = stdFS.GetDevicesList();
        std::move(he2Res.begin(), he2Res.end(), std::back_inserter(res));
        return res;
    }

    IGFD::Utils::PathStruct ParsePathFileName(const std::string& vPathFileName) override {
        return hh::fnd::FileSystem::GetInstance()->GetFileSystem(hh::fnd::InplaceTempUri<>{ vPathFileName.c_str(), vPathFileName.size() }) ? he2FS.ParsePathFileName(vPathFileName) : stdFS.ParsePathFileName(vPathFileName);
    }

    std::vector<IGFD::FileInfos> ScanDirectory(const std::string& vPath) override {
        return hh::fnd::FileSystem::GetInstance()->GetFileSystem(hh::fnd::InplaceTempUri<>{ vPath.c_str(), vPath.size() }) ? he2FS.ScanDirectory(vPath) : stdFS.ScanDirectory(vPath);
    }

    bool IsDirectory(const std::string& vFilePathName) override {
        return hh::fnd::FileSystem::GetInstance()->GetFileSystem(hh::fnd::InplaceTempUri<>{ vFilePathName.c_str(), vFilePathName.size() }) ? he2FS.IsDirectory(vFilePathName) : stdFS.IsDirectory(vFilePathName);
    }
};
