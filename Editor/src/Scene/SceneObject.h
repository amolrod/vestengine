#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Vest {

struct SceneObject {
    enum class MeshType { Triangle = 0, Quad };

    std::string name;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec4 color = glm::vec4(1.0f);
    bool textured = false;
    MeshType mesh = MeshType::Triangle;
};

}  // namespace Vest
