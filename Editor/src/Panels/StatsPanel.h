#pragma once

#include <cstdint>
#include <string>

namespace Vest {

class StatsPanel {
public:
    explicit StatsPanel(std::string title = "Stats") : m_Title(std::move(title)) {}

    void Update(float fps, uint32_t drawCalls) {
        m_FPS = fps;
        m_DrawCalls = drawCalls;
    }

    void OnImGuiRender();

private:
    std::string m_Title;
    float m_FPS = 0.0f;
    uint32_t m_DrawCalls = 0;
};

}  // namespace Vest
