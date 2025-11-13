#pragma once

#include <string>
#include <vector>
#include <utility>

namespace Vest {

class ContentBrowserPanel {
public:
    explicit ContentBrowserPanel(std::string title = "Content Browser");

    void OnImGuiRender();

private:
    std::string m_Title;
    std::vector<std::string> m_Entries;
};

}  // namespace Vest
