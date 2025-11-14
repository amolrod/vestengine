#include <gtest/gtest.h>
#include "Commands/CommandManager.h"
#include "Commands/TransformCommand.h"
#include "Commands/EntityCommands.h"
#include "Commands/MacroCommand.h"
#include "Scene/SceneObject.h"

namespace Vest {

class CommandTests : public ::testing::Test {
protected:
    std::vector<SceneObject> scene;
    
    void SetUp() override {
        scene.clear();
        
        // Create test objects
        SceneObject obj1;
        obj1.name = "Object1";
        obj1.position = glm::vec3(0.0f, 0.0f, 0.0f);
        obj1.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        obj1.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        obj1.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        scene.push_back(obj1);
        
        SceneObject obj2;
        obj2.name = "Object2";
        obj2.position = glm::vec3(1.0f, 1.0f, 1.0f);
        scene.push_back(obj2);
    }
};

// TransformCommand Tests
TEST_F(CommandTests, TransformCommandPosition) {
    glm::vec3 oldPos = scene[0].position;
    glm::vec3 newPos(5.0f, 5.0f, 5.0f);
    
    auto cmd = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Position, oldPos, newPos);
    
    EXPECT_TRUE(cmd->Execute());
    EXPECT_FLOAT_EQ(scene[0].position.x, newPos.x);
    EXPECT_FLOAT_EQ(scene[0].position.y, newPos.y);
    EXPECT_FLOAT_EQ(scene[0].position.z, newPos.z);
    
    EXPECT_TRUE(cmd->Undo());
    EXPECT_FLOAT_EQ(scene[0].position.x, oldPos.x);
    EXPECT_FLOAT_EQ(scene[0].position.y, oldPos.y);
    EXPECT_FLOAT_EQ(scene[0].position.z, oldPos.z);
}

TEST_F(CommandTests, TransformCommandRotation) {
    glm::vec3 oldRot = scene[0].rotation;
    glm::vec3 newRot(0.0f, 45.0f, 0.0f);
    
    auto cmd = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Rotation, oldRot, newRot);
    
    EXPECT_TRUE(cmd->Execute());
    EXPECT_FLOAT_EQ(scene[0].rotation.x, newRot.x);
    EXPECT_FLOAT_EQ(scene[0].rotation.y, newRot.y);
    EXPECT_FLOAT_EQ(scene[0].rotation.z, newRot.z);
    
    EXPECT_TRUE(cmd->Undo());
    EXPECT_FLOAT_EQ(scene[0].rotation.x, oldRot.x);
    EXPECT_FLOAT_EQ(scene[0].rotation.y, oldRot.y);
    EXPECT_FLOAT_EQ(scene[0].rotation.z, oldRot.z);
}

TEST_F(CommandTests, TransformCommandScale) {
    glm::vec3 oldScale = scene[0].scale;
    glm::vec3 newScale(2.0f, 2.0f, 2.0f);
    
    auto cmd = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Scale, oldScale, newScale);
    
    EXPECT_TRUE(cmd->Execute());
    EXPECT_FLOAT_EQ(scene[0].scale.x, newScale.x);
    EXPECT_FLOAT_EQ(scene[0].scale.y, newScale.y);
    EXPECT_FLOAT_EQ(scene[0].scale.z, newScale.z);
    
    EXPECT_TRUE(cmd->Undo());
    EXPECT_FLOAT_EQ(scene[0].scale.x, oldScale.x);
    EXPECT_FLOAT_EQ(scene[0].scale.y, oldScale.y);
    EXPECT_FLOAT_EQ(scene[0].scale.z, oldScale.z);
}

TEST_F(CommandTests, TransformCommandMerge) {
    auto cmd1 = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Position, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    auto cmd2 = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Position, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    
    EXPECT_TRUE(cmd1->CanMergeWith(cmd2.get()));
    EXPECT_TRUE(cmd1->MergeWith(cmd2.get()));
}

TEST_F(CommandTests, TransformCommandNoMergeDifferentTypes) {
    auto cmd1 = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Position, glm::vec3(0.0f), glm::vec3(1.0f));
    
    auto cmd2 = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Rotation, glm::vec3(0.0f), glm::vec3(45.0f, 0.0f, 0.0f));
    
    EXPECT_FALSE(cmd1->CanMergeWith(cmd2.get()));
}

// CreateEntityCommand Tests
TEST_F(CommandTests, CreateEntityCommand) {
    SceneObject newObj;
    newObj.name = "NewObject";
    
    size_t originalSize = scene.size();
    auto cmd = CreateScope<CreateEntityCommand>(&scene, newObj);
    
    EXPECT_TRUE(cmd->Execute());
    EXPECT_EQ(scene.size(), originalSize + 1);
    EXPECT_EQ(scene.back().name, "NewObject");
    
    EXPECT_TRUE(cmd->Undo());
    EXPECT_EQ(scene.size(), originalSize);
}

// DeleteEntityCommand Tests
TEST_F(CommandTests, DeleteEntityCommand) {
    size_t originalSize = scene.size();
    std::string deletedName = scene[0].name;
    
    auto cmd = CreateScope<DeleteEntityCommand>(&scene, 0);
    
    EXPECT_TRUE(cmd->Execute());
    EXPECT_EQ(scene.size(), originalSize - 1);
    
    EXPECT_TRUE(cmd->Undo());
    EXPECT_EQ(scene.size(), originalSize);
    EXPECT_EQ(scene[0].name, deletedName);
}

TEST_F(CommandTests, DeleteEntityCommandInvalidIndex) {
    auto cmd = CreateScope<DeleteEntityCommand>(&scene, 999);
    EXPECT_FALSE(cmd->Execute());
}

// ModifyColorCommand Tests
TEST_F(CommandTests, ModifyColorCommand) {
    glm::vec4 oldColor = scene[0].color;
    glm::vec4 newColor(0.0f, 1.0f, 0.0f, 1.0f);
    
    auto cmd = CreateScope<ModifyColorCommand>(&scene, 0, oldColor, newColor);
    
    EXPECT_TRUE(cmd->Execute());
    EXPECT_FLOAT_EQ(scene[0].color.r, newColor.r);
    EXPECT_FLOAT_EQ(scene[0].color.g, newColor.g);
    EXPECT_FLOAT_EQ(scene[0].color.b, newColor.b);
    EXPECT_FLOAT_EQ(scene[0].color.a, newColor.a);
    
    EXPECT_TRUE(cmd->Undo());
    EXPECT_FLOAT_EQ(scene[0].color.r, oldColor.r);
    EXPECT_FLOAT_EQ(scene[0].color.g, oldColor.g);
    EXPECT_FLOAT_EQ(scene[0].color.b, oldColor.b);
    EXPECT_FLOAT_EQ(scene[0].color.a, oldColor.a);
}

// CommandManager Tests
TEST_F(CommandTests, CommandManagerExecute) {
    CommandManager manager;
    
    auto cmd = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Position, glm::vec3(0.0f), glm::vec3(5.0f));
    
    EXPECT_TRUE(manager.ExecuteCommand(std::move(cmd)));
    EXPECT_TRUE(manager.CanUndo());
    EXPECT_FALSE(manager.CanRedo());
}

TEST_F(CommandTests, CommandManagerUndoRedo) {
    CommandManager manager;
    glm::vec3 original = scene[0].position;
    glm::vec3 modified(5.0f, 5.0f, 5.0f);
    
    auto cmd = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Position, original, modified);
    
    manager.ExecuteCommand(std::move(cmd));
    EXPECT_FLOAT_EQ(scene[0].position.x, modified.x);
    EXPECT_FLOAT_EQ(scene[0].position.y, modified.y);
    EXPECT_FLOAT_EQ(scene[0].position.z, modified.z);
    
    EXPECT_TRUE(manager.Undo());
    EXPECT_FLOAT_EQ(scene[0].position.x, original.x);
    EXPECT_FLOAT_EQ(scene[0].position.y, original.y);
    EXPECT_FLOAT_EQ(scene[0].position.z, original.z);
    EXPECT_TRUE(manager.CanRedo());
    
    EXPECT_TRUE(manager.Redo());
    EXPECT_FLOAT_EQ(scene[0].position.x, modified.x);
    EXPECT_FLOAT_EQ(scene[0].position.y, modified.y);
    EXPECT_FLOAT_EQ(scene[0].position.z, modified.z);
}

TEST_F(CommandTests, CommandManagerMultipleCommands) {
    CommandManager manager;
    
    auto cmd1 = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Position, glm::vec3(0.0f), glm::vec3(1.0f));
    auto cmd2 = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Scale, glm::vec3(1.0f), glm::vec3(2.0f));
    
    manager.ExecuteCommand(std::move(cmd1));
    manager.ExecuteCommand(std::move(cmd2));
    
    EXPECT_EQ(manager.GetUndoStackSize(), 2);
    
    manager.Undo();
    EXPECT_FLOAT_EQ(scene[0].scale.x, 1.0f);
    
    manager.Undo();
    EXPECT_FLOAT_EQ(scene[0].position.x, 0.0f);
}

TEST_F(CommandTests, CommandManagerClearHistory) {
    CommandManager manager;
    
    auto cmd = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Position, glm::vec3(0.0f), glm::vec3(1.0f));
    
    manager.ExecuteCommand(std::move(cmd));
    EXPECT_GT(manager.GetUndoStackSize(), 0);
    
    manager.Clear();
    EXPECT_EQ(manager.GetUndoStackSize(), 0);
    EXPECT_EQ(manager.GetRedoStackSize(), 0);
}

TEST_F(CommandTests, CommandManagerHistoryLimit) {
    CommandManager manager(3);  // Limit to 3 commands
    
    // Create commands that WON'T merge (different entities)
    for (int i = 0; i < 5; i++) {
        int entityIndex = i % 2;  // Alternate between entities to prevent merging
        auto cmd = CreateScope<TransformCommand>(&scene, entityIndex, 
            TransformCommand::Type::Position, 
            glm::vec3(static_cast<float>(i)), 
            glm::vec3(static_cast<float>(i + 1)));
        manager.ExecuteCommand(std::move(cmd));
    }
    
    EXPECT_LE(manager.GetUndoStackSize(), 3);
}

TEST_F(CommandTests, CommandManagerRedoClearedAfterNewCommand) {
    CommandManager manager;
    
    auto cmd1 = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Position, glm::vec3(0.0f), glm::vec3(1.0f));
    auto cmd2 = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Position, glm::vec3(0.0f), glm::vec3(2.0f));
    
    manager.ExecuteCommand(std::move(cmd1));
    manager.Undo();
    
    EXPECT_TRUE(manager.CanRedo());
    
    manager.ExecuteCommand(std::move(cmd2));
    EXPECT_FALSE(manager.CanRedo());
}

TEST_F(CommandTests, CommandManagerRegisterExecuted) {
    CommandManager manager;
    glm::vec3 newPos(5.0f, 5.0f, 5.0f);
    
    // Manually change position
    scene[0].position = newPos;
    
    // Register the change without executing
    auto cmd = CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Position, glm::vec3(0.0f), newPos);
    
    EXPECT_TRUE(manager.RegisterExecutedCommand(std::move(cmd)));
    EXPECT_TRUE(manager.CanUndo());
    
    manager.Undo();
    EXPECT_FLOAT_EQ(scene[0].position.x, 0.0f);
    EXPECT_FLOAT_EQ(scene[0].position.y, 0.0f);
    EXPECT_FLOAT_EQ(scene[0].position.z, 0.0f);
}

// MacroCommand Tests
TEST_F(CommandTests, MacroCommandExecution) {
    auto macro = CreateScope<MacroCommand>("Create and Move");
    
    SceneObject newObj;
    newObj.name = "MacroTest";
    
    macro->AddCommand(CreateScope<CreateEntityCommand>(&scene, newObj));
    macro->AddCommand(CreateScope<TransformCommand>(&scene, 2, 
        TransformCommand::Type::Position, glm::vec3(0.0f), glm::vec3(10.0f)));
    
    size_t originalSize = scene.size();
    EXPECT_TRUE(macro->Execute());
    EXPECT_EQ(scene.size(), originalSize + 1);
    EXPECT_FLOAT_EQ(scene.back().position.x, 10.0f);
    EXPECT_FLOAT_EQ(scene.back().position.y, 10.0f);
    EXPECT_FLOAT_EQ(scene.back().position.z, 10.0f);
}

TEST_F(CommandTests, MacroCommandUndo) {
    auto macro = CreateScope<MacroCommand>("Complex Operation");
    
    SceneObject newObj;
    newObj.name = "MacroTest";
    
    macro->AddCommand(CreateScope<CreateEntityCommand>(&scene, newObj));
    macro->AddCommand(CreateScope<TransformCommand>(&scene, 2, 
        TransformCommand::Type::Scale, glm::vec3(1.0f), glm::vec3(3.0f)));
    
    size_t originalSize = scene.size();
    macro->Execute();
    
    EXPECT_TRUE(macro->Undo());
    EXPECT_EQ(scene.size(), originalSize);
}

TEST_F(CommandTests, MacroCommandEmpty) {
    auto macro = CreateScope<MacroCommand>("Empty Macro");
    EXPECT_TRUE(macro->IsEmpty());
    EXPECT_FALSE(macro->Execute());
}

TEST_F(CommandTests, MacroCommandCount) {
    auto macro = CreateScope<MacroCommand>("Multiple Commands");
    
    macro->AddCommand(CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Position, glm::vec3(0.0f), glm::vec3(1.0f)));
    macro->AddCommand(CreateScope<TransformCommand>(&scene, 0, 
        TransformCommand::Type::Rotation, glm::vec3(0.0f), glm::vec3(45.0f)));
    
    EXPECT_EQ(macro->GetCommandCount(), 2);
}

}  // namespace Vest
