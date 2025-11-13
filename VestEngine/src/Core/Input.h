#pragma once

#include <glm/glm.hpp>

namespace Vest {

class Input {
public:
    static bool IsKeyPressed(int keycode);
    static glm::vec2 GetMousePosition();
};

}  // namespace Vest
