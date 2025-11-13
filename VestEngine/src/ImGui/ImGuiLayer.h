#pragma once

#include "Core/Layer.h"

namespace Vest {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Event& event) override;

    void Begin();
    void End();

    void SetBlockEvents(bool block) { m_BlockEvents = block; }

private:
    bool m_BlockEvents = false;
    float m_Time = 0.0f;
};

}  // namespace Vest
