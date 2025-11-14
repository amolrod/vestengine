#include "Serialization/SceneSerializer.h"

#include <fstream>
#include <filesystem>

#include "Core/Log.h"
#include <Scene/SceneObject.h>

namespace Vest {

static constexpr const char* SCENE_VERSION = "1.0";

bool SceneSerializer::Serialize(const std::string& filepath, const std::vector<SceneObject>& objects) {
    VEST_CORE_INFO("Serializing scene to: {0}", filepath);
    
    // Create backup if file exists
    if (std::filesystem::exists(filepath)) {
        std::string backupPath = filepath + ".bak";
        try {
            std::filesystem::copy_file(filepath, backupPath, std::filesystem::copy_options::overwrite_existing);
            VEST_CORE_INFO("Created backup: {0}", backupPath);
        } catch (const std::exception& e) {
            VEST_CORE_WARN("Failed to create backup: {0}", e.what());
        }
    }
    
    nlohmann::json json;
    json["version"] = SCENE_VERSION;
    json["objects"] = nlohmann::json::array();
    
    for (const auto& obj : objects) {
        nlohmann::json entry;
        entry["name"] = obj.name;
        entry["position"] = {obj.position.x, obj.position.y, obj.position.z};
        entry["rotation"] = {obj.rotation.x, obj.rotation.y, obj.rotation.z};
        entry["scale"] = {obj.scale.x, obj.scale.y, obj.scale.z};
        entry["color"] = {obj.color.r, obj.color.g, obj.color.b, obj.color.a};
        entry["textured"] = obj.textured;
        entry["mesh"] = static_cast<int>(obj.mesh);
        json["objects"].push_back(entry);
    }

    std::ofstream out(filepath);
    if (!out.is_open()) {
        VEST_CORE_ERROR("Failed to open file for writing: {0}", filepath);
        return false;
    }
    
    out << json.dump(4);
    VEST_CORE_INFO("Scene serialized successfully: {0} objects", objects.size());
    return true;
}

bool SceneSerializer::Deserialize(const std::string& filepath, std::vector<SceneObject>& outObjects) {
    VEST_CORE_INFO("Deserializing scene from: {0}", filepath);
    
    std::ifstream in(filepath);
    if (!in.is_open()) {
        VEST_CORE_ERROR("Failed to open file for reading: {0}", filepath);
        return false;
    }

    nlohmann::json json;
    try {
        in >> json;
    } catch (const nlohmann::json::exception& e) {
        VEST_CORE_ERROR("Failed to parse JSON: {0}", e.what());
        return false;
    }
    
    // Validate version
    if (!json.contains("version")) {
        VEST_CORE_WARN("Scene file missing version field, assuming legacy format");
    } else {
        std::string version = json["version"];
        if (version != SCENE_VERSION) {
            VEST_CORE_WARN("Scene version mismatch: file={0}, expected={1}", version, SCENE_VERSION);
        }
    }
    
    // Validate structure
    if (!json.contains("objects")) {
        VEST_CORE_ERROR("Invalid scene file: missing 'objects' array");
        return false;
    }
    
    if (!json["objects"].is_array()) {
        VEST_CORE_ERROR("Invalid scene file: 'objects' is not an array");
        return false;
    }

    outObjects.clear();
    
    for (const auto& entry : json["objects"]) {
        try {
            SceneObject obj;
            obj.name = entry.value("name", "Entity");
            
            // Validate and parse position
            if (!entry.contains("position") || !entry["position"].is_array() || entry["position"].size() != 3) {
                VEST_CORE_WARN("Invalid position data for object '{0}', using default", obj.name);
                obj.position = glm::vec3(0.0f);
            } else {
                auto pos = entry["position"];
                obj.position = {pos[0].get<float>(), pos[1].get<float>(), pos[2].get<float>()};
            }
            
            // Validate and parse rotation
            if (!entry.contains("rotation") || !entry["rotation"].is_array() || entry["rotation"].size() != 3) {
                VEST_CORE_WARN("Invalid rotation data for object '{0}', using default", obj.name);
                obj.rotation = glm::vec3(0.0f);
            } else {
                auto rot = entry["rotation"];
                obj.rotation = {rot[0].get<float>(), rot[1].get<float>(), rot[2].get<float>()};
            }
            
            // Validate and parse scale
            if (!entry.contains("scale") || !entry["scale"].is_array() || entry["scale"].size() != 3) {
                VEST_CORE_WARN("Invalid scale data for object '{0}', using default", obj.name);
                obj.scale = glm::vec3(1.0f);
            } else {
                auto scale = entry["scale"];
                obj.scale = {scale[0].get<float>(), scale[1].get<float>(), scale[2].get<float>()};
            }
            
            // Validate and parse color
            if (!entry.contains("color") || !entry["color"].is_array() || entry["color"].size() != 4) {
                VEST_CORE_WARN("Invalid color data for object '{0}', using default", obj.name);
                obj.color = glm::vec4(1.0f);
            } else {
                auto color = entry["color"];
                obj.color = {color[0].get<float>(), color[1].get<float>(), color[2].get<float>(), color[3].get<float>()};
            }
            
            obj.textured = entry.value("textured", false);
            obj.mesh = static_cast<SceneObject::MeshType>(entry.value("mesh", 0));
            
            outObjects.push_back(obj);
        } catch (const std::exception& e) {
            VEST_CORE_ERROR("Failed to deserialize object: {0}", e.what());
            continue;  // Skip invalid objects
        }
    }

    VEST_CORE_INFO("Scene deserialized successfully: {0} objects", outObjects.size());
    return true;
}

}  // namespace Vest
