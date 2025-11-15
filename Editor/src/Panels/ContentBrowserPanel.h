#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>

#include "Core/Base.h"
#include "Rendering/Texture.h"

namespace Vest {

struct AssetMetadata {
    std::string name;
    std::filesystem::path path;
    std::filesystem::file_time_type lastModified;
    uintmax_t size;
    bool isDirectory;
    std::string extension;
};

class ContentBrowserPanel {
public:
    explicit ContentBrowserPanel(const std::filesystem::path& assetsPath = "assets");

    void OnImGuiRender();
    
    const std::filesystem::path& GetCurrentDirectory() const { return m_CurrentDirectory; }
    void SetCurrentDirectory(const std::filesystem::path& path);

private:
    void RenderTopBar();
    void RenderDirectoryTree();
    void RenderAssetGrid();
    void RenderAssetItem(const AssetMetadata& asset);
    void RenderContextMenu();
    
    void NavigateToDirectory(const std::filesystem::path& path);
    void NavigateBack();
    void NavigateUp();
    void RefreshAssets();
    
    Ref<Texture2D> GetIconForFileType(const std::string& extension);
    std::string FormatFileSize(uintmax_t bytes);

private:
    std::filesystem::path m_AssetsPath;
    std::filesystem::path m_CurrentDirectory;
    std::vector<std::filesystem::path> m_NavigationHistory;
    int m_HistoryIndex = -1;
    
    std::vector<AssetMetadata> m_CurrentAssets;
    AssetMetadata* m_SelectedAsset = nullptr;
    
    // Icons
    Ref<Texture2D> m_FolderIcon;
    Ref<Texture2D> m_FileIcon;
    Ref<Texture2D> m_ImageIcon;
    Ref<Texture2D> m_SceneIcon;
    std::unordered_map<std::string, Ref<Texture2D>> m_IconCache;
    
    // UI State
    float m_ThumbnailSize = 80.0f;
    float m_Padding = 16.0f;
    bool m_ShowHiddenFiles = false;
    std::string m_SearchFilter;
};

}  // namespace Vest
