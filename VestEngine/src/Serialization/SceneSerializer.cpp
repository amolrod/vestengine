#include "Serialization/SceneSerializer.h"

#include <fstream>

#include <Scene/SceneObject.h>

namespace Vest {

bool SceneSerializer::Serialize(const std::string& filepath, const std::vector<SceneObject>& objects) {
    nlohmann::json json;
    json["objects"] = nlohmann::json::array();
    for (const auto& obj : objects) {
        nlohmann::json entry;
        entry["name"] = obj.name;
        entry["position"] = {obj.position.x, obj.position.y, obj.position.z};
        entry["rotation"] = {obj.rotation.x, obj.rotation.y, obj.rotation.z};
        entry["scale"] = {obj.scale.x, obj.scale.y, obj.scale.z};
        entry["color"] = {obj.color.r, obj.color.g, obj.color.b, obj.color.a};
        entry["textured"] = obj.textured;
        json["objects"].push_back(entry);
    }

    std::ofstream out(filepath);
    if (!out.is_open()) {
        return false;
    }
    out << json.dump(4);
    return true;
}

bool SceneSerializer::Deserialize(const std::string& filepath, std::vector<SceneObject>& outObjects) {
    std::ifstream in(filepath);
    if (!in.is_open()) {
        return false;
    }

    nlohmann::json json;
    in >> json;
    if (!json.contains("objects")) {
        return false;
    }

    outObjects.clear();
    for (const auto& entry : json["objects"]) {
        SceneObject obj;
        obj.name = entry.value("name", "Entity");
        auto pos = entry["position"];
        obj.position = {pos[0].get<float>(), pos[1].get<float>(), pos[2].get<float>()};
        auto rot = entry["rotation"];
        obj.rotation = {rot[0].get<float>(), rot[1].get<float>(), rot[2].get<float>()};
        auto scale = entry["scale"];
        obj.scale = {scale[0].get<float>(), scale[1].get<float>(), scale[2].get<float>()};
        auto color = entry["color"];
        obj.color = {color[0].get<float>(), color[1].get<float>(), color[2].get<float>(), color[3].get<float>()};
        obj.textured = entry.value("textured", false);
        outObjects.push_back(obj);
    }

    return true;
}

}  // namespace Vest
