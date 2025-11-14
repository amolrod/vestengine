#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <Scene/SceneObject.h>

namespace Vest {

class SceneSerializer {
public:
    static bool Serialize(const std::string& filepath, const std::vector<SceneObject>& objects);
    static bool Deserialize(const std::string& filepath, std::vector<SceneObject>& outObjects);
};

}  // namespace Vest
