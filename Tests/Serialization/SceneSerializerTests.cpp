#include <gtest/gtest.h>
#include "Serialization/SceneSerializer.h"
#include "Scene/SceneObject.h"
#include <filesystem>
#include <fstream>

namespace Vest {

class SceneSerializerTests : public ::testing::Test {
protected:
    const std::string testFile = "test_scene.json";
    const std::string backupFile = testFile + ".bak";
    
    void SetUp() override {
        CleanupFiles();
    }

    void TearDown() override {
        CleanupFiles();
    }

    void CleanupFiles() {
        if (std::filesystem::exists(testFile)) {
            std::filesystem::remove(testFile);
        }
        if (std::filesystem::exists(backupFile)) {
            std::filesystem::remove(backupFile);
        }
    }

    std::vector<SceneObject> CreateTestScene() {
        std::vector<SceneObject> scene;
        
        SceneObject obj1;
        obj1.name = "TestObject1";
        obj1.position = glm::vec3(1.0f, 2.0f, 3.0f);
        obj1.rotation = glm::vec3(0.0f, 45.0f, 0.0f);
        obj1.scale = glm::vec3(1.5f, 1.5f, 1.5f);
        obj1.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        obj1.mesh = SceneObject::MeshType::Triangle;
        obj1.textured = false;
        scene.push_back(obj1);
        
        SceneObject obj2;
        obj2.name = "TestObject2";
        obj2.position = glm::vec3(-2.0f, 0.0f, 1.0f);
        obj2.rotation = glm::vec3(0.0f, 0.0f, 90.0f);
        obj2.scale = glm::vec3(2.0f, 0.5f, 1.0f);
        obj2.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        obj2.mesh = SceneObject::MeshType::Quad;
        obj2.textured = true;
        scene.push_back(obj2);
        
        return scene;
    }
};

TEST_F(SceneSerializerTests, SerializeEmptyScene) {
    std::vector<SceneObject> emptyScene;
    EXPECT_TRUE(SceneSerializer::Serialize(testFile, emptyScene));
    EXPECT_TRUE(std::filesystem::exists(testFile));
}

TEST_F(SceneSerializerTests, SerializeValidScene) {
    auto scene = CreateTestScene();
    EXPECT_TRUE(SceneSerializer::Serialize(testFile, scene));
    EXPECT_TRUE(std::filesystem::exists(testFile));
}

TEST_F(SceneSerializerTests, DeserializeEmptyScene) {
    std::vector<SceneObject> emptyScene;
    SceneSerializer::Serialize(testFile, emptyScene);
    
    std::vector<SceneObject> loaded;
    EXPECT_TRUE(SceneSerializer::Deserialize(testFile, loaded));
    EXPECT_EQ(loaded.size(), 0);
}

TEST_F(SceneSerializerTests, DeserializeValidScene) {
    auto scene = CreateTestScene();
    SceneSerializer::Serialize(testFile, scene);
    
    std::vector<SceneObject> loaded;
    EXPECT_TRUE(SceneSerializer::Deserialize(testFile, loaded));
    EXPECT_EQ(loaded.size(), scene.size());
}

TEST_F(SceneSerializerTests, PreserveObjectProperties) {
    auto scene = CreateTestScene();
    SceneSerializer::Serialize(testFile, scene);
    
    std::vector<SceneObject> loaded;
    SceneSerializer::Deserialize(testFile, loaded);
    
    ASSERT_EQ(loaded.size(), 2);
    
    // Check first object
    EXPECT_EQ(loaded[0].name, "TestObject1");
    EXPECT_FLOAT_EQ(loaded[0].position.x, 1.0f);
    EXPECT_FLOAT_EQ(loaded[0].position.y, 2.0f);
    EXPECT_FLOAT_EQ(loaded[0].position.z, 3.0f);
    EXPECT_FLOAT_EQ(loaded[0].rotation.y, 45.0f);
    EXPECT_FLOAT_EQ(loaded[0].scale.x, 1.5f);
    EXPECT_EQ(loaded[0].mesh, SceneObject::MeshType::Triangle);
    EXPECT_FALSE(loaded[0].textured);
    
    // Check second object
    EXPECT_EQ(loaded[1].name, "TestObject2");
    EXPECT_FLOAT_EQ(loaded[1].position.x, -2.0f);
    EXPECT_FLOAT_EQ(loaded[1].rotation.z, 90.0f);
    EXPECT_EQ(loaded[1].mesh, SceneObject::MeshType::Quad);
    EXPECT_TRUE(loaded[1].textured);
}

TEST_F(SceneSerializerTests, BackupCreation) {
    // Create initial scene
    auto scene = CreateTestScene();
    SceneSerializer::Serialize(testFile, scene);
    
    // Modify and save again (should create backup)
    scene[0].name = "Modified";
    SceneSerializer::Serialize(testFile, scene);
    
    EXPECT_TRUE(std::filesystem::exists(backupFile));
}

TEST_F(SceneSerializerTests, InvalidFilePath) {
    std::vector<SceneObject> scene;
    EXPECT_FALSE(SceneSerializer::Serialize("/invalid/path/test.json", scene));
}

TEST_F(SceneSerializerTests, DeserializeNonexistentFile) {
    std::vector<SceneObject> loaded;
    EXPECT_FALSE(SceneSerializer::Deserialize("nonexistent.json", loaded));
}

TEST_F(SceneSerializerTests, DeserializeCorruptedJSON) {
    // Create corrupted JSON file
    std::ofstream file(testFile);
    file << "{ invalid json content }}}";
    file.close();
    
    std::vector<SceneObject> loaded;
    EXPECT_FALSE(SceneSerializer::Deserialize(testFile, loaded));
}

TEST_F(SceneSerializerTests, VersionCheck) {
    auto scene = CreateTestScene();
    SceneSerializer::Serialize(testFile, scene);
    
    // Read file and check version field
    std::ifstream file(testFile);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    EXPECT_NE(content.find("\"version\""), std::string::npos);
    EXPECT_NE(content.find("\"1.0\""), std::string::npos);
}

TEST_F(SceneSerializerTests, RoundTripConsistency) {
    auto originalScene = CreateTestScene();
    
    // Save and load multiple times
    for (int i = 0; i < 3; i++) {
        SceneSerializer::Serialize(testFile, originalScene);
        
        std::vector<SceneObject> loadedScene;
        SceneSerializer::Deserialize(testFile, loadedScene);
        
        ASSERT_EQ(loadedScene.size(), originalScene.size());
        
        for (size_t j = 0; j < originalScene.size(); j++) {
            EXPECT_EQ(loadedScene[j].name, originalScene[j].name);
            EXPECT_EQ(loadedScene[j].mesh, originalScene[j].mesh);
            EXPECT_EQ(loadedScene[j].textured, originalScene[j].textured);
        }
        
        originalScene = loadedScene;
    }
}

}  // namespace Vest
