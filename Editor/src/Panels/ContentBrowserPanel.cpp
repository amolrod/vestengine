#include "Panels/ContentBrowserPanel.h"

#include <imgui.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "Core/Log.h"

namespace Vest {

ContentBrowserPanel::ContentBrowserPanel(const std::filesystem::path& assetsPath)
    : m_AssetsPath(assetsPath)
    , m_CurrentDirectory(assetsPath)
{
    if (!std::filesystem::exists(m_AssetsPath)) {
        std::filesystem::create_directories(m_AssetsPath);
        VEST_CORE_INFO("Created assets directory: {0}", m_AssetsPath.string());
    }
    
    // Create default texture for icons (placeholder - will be replaced with actual textures)
    // For now, we'll use nullptr and render colored rectangles
    
    RefreshAssets();
    
    VEST_CORE_INFO("Content Browser initialized at: {0}", m_CurrentDirectory.string());
}

void ContentBrowserPanel::OnImGuiRender() {
    ImGui::Begin("Content Browser");
    
    RenderTopBar();
    
    ImGui::Separator();
    
    // Two-column layout: Directory tree on left, assets on right
    ImGui::BeginChild("Directory Tree", ImVec2(200, 0), true);
    RenderDirectoryTree();
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    ImGui::BeginChild("Asset View", ImVec2(0, 0), true);
    RenderAssetGrid();
    ImGui::EndChild();
    
    RenderContextMenu();
    
    ImGui::End();
}

void ContentBrowserPanel::RenderTopBar() {
    // Navigation buttons
    if (ImGui::Button("<-")) {
        NavigateBack();
    }
    ImGui::SameLine();
    
    if (ImGui::Button("^")) {
        NavigateUp();
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Refresh")) {
        RefreshAssets();
    }
    ImGui::SameLine();
    
    // Current path
    ImGui::Text("Path: %s", m_CurrentDirectory.string().c_str());
    
    // Search bar
    ImGui::SameLine();
    char searchBuffer[256];
    strncpy(searchBuffer, m_SearchFilter.c_str(), sizeof(searchBuffer));
    searchBuffer[sizeof(searchBuffer) - 1] = '\0';
    
    if (ImGui::InputText("##Search", searchBuffer, sizeof(searchBuffer))) {
        m_SearchFilter = searchBuffer;
    }
    
    // Thumbnail size slider
    ImGui::SliderFloat("Size", &m_ThumbnailSize, 50.0f, 150.0f);
    ImGui::SameLine();
    ImGui::Checkbox("Show Hidden", &m_ShowHiddenFiles);
}

void ContentBrowserPanel::RenderDirectoryTree() {
    ImGui::Text("Directories");
    ImGui::Separator();
    
    // Render directory tree recursively
    std::function<void(const std::filesystem::path&)> renderTree = [&](const std::filesystem::path& path) {
        if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
            return;
        }
        
        try {
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                if (!entry.is_directory()) continue;
                
                const auto& dirPath = entry.path();
                std::string dirName = dirPath.filename().string();
                
                // Skip hidden directories if not showing them
                if (!m_ShowHiddenFiles && dirName[0] == '.') continue;
                
                bool isOpen = ImGui::TreeNodeEx(dirName.c_str(), 
                    ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick);
                
                if (ImGui::IsItemClicked()) {
                    NavigateToDirectory(dirPath);
                }
                
                if (isOpen) {
                    renderTree(dirPath);
                    ImGui::TreePop();
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            VEST_CORE_ERROR("Error reading directory tree: {0}", e.what());
        }
    };
    
    renderTree(m_AssetsPath);
}

void ContentBrowserPanel::RenderAssetGrid() {
    float cellSize = m_ThumbnailSize + m_Padding;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = static_cast<int>(panelWidth / cellSize);
    if (columnCount < 1) columnCount = 1;
    
    ImGui::Columns(columnCount, nullptr, false);
    
    for (auto& asset : m_CurrentAssets) {
        // Apply search filter
        if (!m_SearchFilter.empty()) {
            std::string lowerName = asset.name;
            std::string lowerFilter = m_SearchFilter;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            std::transform(lowerFilter.begin(), lowerFilter.end(), lowerFilter.begin(), ::tolower);
            
            if (lowerName.find(lowerFilter) == std::string::npos) {
                continue;
            }
        }
        
        RenderAssetItem(asset);
        ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
}

void ContentBrowserPanel::RenderAssetItem(const AssetMetadata& asset) {
    ImGui::PushID(asset.path.string().c_str());
    
    // Icon/Thumbnail
    ImVec2 thumbnailSize(m_ThumbnailSize, m_ThumbnailSize);
    
    // Draw colored rectangle as placeholder icon
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    ImU32 iconColor = asset.isDirectory ? 
        IM_COL32(255, 200, 0, 255) :  // Yellow for folders
        IM_COL32(100, 150, 255, 255);  // Blue for files
    
    if (!asset.isDirectory) {
        if (asset.extension == ".json") iconColor = IM_COL32(0, 255, 100, 255);
        else if (asset.extension == ".png" || asset.extension == ".jpg") iconColor = IM_COL32(255, 100, 255, 255);
    }
    
    drawList->AddRectFilled(cursorPos, 
        ImVec2(cursorPos.x + thumbnailSize.x, cursorPos.y + thumbnailSize.y), 
        iconColor, 4.0f);
    
    // Make it clickable
    ImGui::InvisibleButton("##thumbnail", thumbnailSize);
    
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
        if (asset.isDirectory) {
            NavigateToDirectory(asset.path);
        } else {
            VEST_CORE_INFO("Opening asset: {0}", asset.path.string());
            // TODO: Open asset in appropriate editor
        }
    }
    
    // Selection highlight
    if (m_SelectedAsset && m_SelectedAsset->path == asset.path) {
        drawList->AddRect(cursorPos, 
            ImVec2(cursorPos.x + thumbnailSize.x, cursorPos.y + thumbnailSize.y), 
            IM_COL32(255, 255, 0, 255), 4.0f, 0, 3.0f);
    }
    
    if (ImGui::IsItemClicked()) {
        m_SelectedAsset = const_cast<AssetMetadata*>(&asset);
    }
    
    // Drag source
    if (ImGui::BeginDragDropSource()) {
        const std::string pathStr = asset.path.string();
        ImGui::SetDragDropPayload("ASSET_PATH", pathStr.c_str(), pathStr.size() + 1);
        ImGui::Text("%s", asset.name.c_str());
        ImGui::EndDragDropSource();
    }
    
    // Asset name (wrapped text)
    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + thumbnailSize.x);
    ImGui::TextWrapped("%s", asset.name.c_str());
    ImGui::PopTextWrapPos();
    
    // Tooltip with details
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("Name: %s", asset.name.c_str());
        if (!asset.isDirectory) {
            ImGui::Text("Type: %s", asset.extension.c_str());
            ImGui::Text("Size: %s", FormatFileSize(asset.size).c_str());
        }
        ImGui::EndTooltip();
    }
    
    ImGui::PopID();
}

void ContentBrowserPanel::RenderContextMenu() {
    if (ImGui::BeginPopupContextWindow("AssetContextMenu", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
        if (ImGui::MenuItem("New Folder")) {
            try {
                auto newFolderPath = m_CurrentDirectory / "NewFolder";
                int counter = 1;
                while (std::filesystem::exists(newFolderPath)) {
                    newFolderPath = m_CurrentDirectory / ("NewFolder" + std::to_string(counter++));
                }
                std::filesystem::create_directory(newFolderPath);
                RefreshAssets();
                VEST_CORE_INFO("Created folder: {0}", newFolderPath.string());
            } catch (const std::filesystem::filesystem_error& e) {
                VEST_CORE_ERROR("Failed to create folder: {0}", e.what());
            }
        }
        
        if (ImGui::MenuItem("Refresh")) {
            RefreshAssets();
        }
        
        ImGui::Separator();
        
        if (m_SelectedAsset) {
            if (ImGui::MenuItem("Delete")) {
                try {
                    if (m_SelectedAsset->isDirectory) {
                        std::filesystem::remove_all(m_SelectedAsset->path);
                    } else {
                        std::filesystem::remove(m_SelectedAsset->path);
                    }
                    RefreshAssets();
                    m_SelectedAsset = nullptr;
                    VEST_CORE_INFO("Deleted asset");
                } catch (const std::filesystem::filesystem_error& e) {
                    VEST_CORE_ERROR("Failed to delete asset: {0}", e.what());
                }
            }
            
            if (ImGui::MenuItem("Rename")) {
                // TODO: Implement rename dialog
                VEST_CORE_INFO("Rename not yet implemented");
            }
        }
        
        ImGui::EndPopup();
    }
}

void ContentBrowserPanel::NavigateToDirectory(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        VEST_CORE_WARN("Cannot navigate to invalid directory: {0}", path.string());
        return;
    }
    
    // Add to history
    if (m_HistoryIndex < static_cast<int>(m_NavigationHistory.size()) - 1) {
        m_NavigationHistory.erase(m_NavigationHistory.begin() + m_HistoryIndex + 1, m_NavigationHistory.end());
    }
    
    m_NavigationHistory.push_back(path);
    m_HistoryIndex = static_cast<int>(m_NavigationHistory.size()) - 1;
    
    m_CurrentDirectory = path;
    m_SelectedAsset = nullptr;
    RefreshAssets();
    
    VEST_CORE_TRACE("Navigated to: {0}", path.string());
}

void ContentBrowserPanel::NavigateBack() {
    if (m_HistoryIndex > 0) {
        m_HistoryIndex--;
        m_CurrentDirectory = m_NavigationHistory[m_HistoryIndex];
        m_SelectedAsset = nullptr;
        RefreshAssets();
    }
}

void ContentBrowserPanel::NavigateUp() {
    if (m_CurrentDirectory != m_AssetsPath && m_CurrentDirectory.has_parent_path()) {
        NavigateToDirectory(m_CurrentDirectory.parent_path());
    }
}

void ContentBrowserPanel::RefreshAssets() {
    m_CurrentAssets.clear();
    
    if (!std::filesystem::exists(m_CurrentDirectory)) {
        VEST_CORE_ERROR("Current directory does not exist: {0}", m_CurrentDirectory.string());
        return;
    }
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
            AssetMetadata metadata;
            metadata.path = entry.path();
            metadata.name = entry.path().filename().string();
            metadata.isDirectory = entry.is_directory();
            
            // Skip hidden files if not showing them
            if (!m_ShowHiddenFiles && metadata.name[0] == '.') continue;
            
            if (!metadata.isDirectory) {
                metadata.extension = entry.path().extension().string();
                metadata.size = entry.file_size();
            }
            
            metadata.lastModified = entry.last_write_time();
            
            m_CurrentAssets.push_back(metadata);
        }
        
        // Sort: directories first, then alphabetically
        std::sort(m_CurrentAssets.begin(), m_CurrentAssets.end(), 
            [](const AssetMetadata& a, const AssetMetadata& b) {
                if (a.isDirectory != b.isDirectory) {
                    return a.isDirectory;
                }
                return a.name < b.name;
            });
            
    } catch (const std::filesystem::filesystem_error& e) {
        VEST_CORE_ERROR("Error refreshing assets: {0}", e.what());
    }
}

void ContentBrowserPanel::SetCurrentDirectory(const std::filesystem::path& path) {
    NavigateToDirectory(path);
}

Ref<Texture2D> ContentBrowserPanel::GetIconForFileType(const std::string& extension) {
    // TODO: Load actual texture icons
    return nullptr;
}

std::string ContentBrowserPanel::FormatFileSize(uintmax_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        unitIndex++;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << " " << units[unitIndex];
    return oss.str();
}

}  // namespace Vest
