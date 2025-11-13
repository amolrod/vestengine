#pragma once

namespace Vest {

class Timestep {
public:
    constexpr Timestep() = default;
    constexpr explicit Timestep(float time) : m_Time(time) {}

    constexpr float GetSeconds() const { return m_Time; }
    constexpr float GetMilliseconds() const { return m_Time * 1000.0f; }

private:
    float m_Time = 0.0f;
};

}  // namespace Vest
