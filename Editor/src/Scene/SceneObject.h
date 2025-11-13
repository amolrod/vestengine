#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Vest {

struct SceneObject {
    std::string name;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    bool textured = false;
};

}  // namespace Vest
