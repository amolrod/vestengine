#include <gtest/gtest.h>
#include "Core/Log.h"
#include <fstream>
#include <filesystem>

namespace Vest {

class LogTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Logger already initialized in main()
        
        // Remove old log file if exists
        if (std::filesystem::exists("VestEngine.log")) {
            std::filesystem::remove("VestEngine.log");
        }
    }

    void TearDown() override {
        // Cleanup
        if (std::filesystem::exists("VestEngine.log")) {
            std::filesystem::remove("VestEngine.log");
        }
    }
};

TEST_F(LogTests, LoggerInitialization) {
    // Logger is already initialized, just check it's valid
    EXPECT_NE(Log::GetCoreLogger(), nullptr);
    EXPECT_NE(Log::GetClientLogger(), nullptr);
}

TEST_F(LogTests, CoreLoggerOutput) {
    EXPECT_NO_THROW(VEST_CORE_INFO("Test info message"));
    EXPECT_NO_THROW(VEST_CORE_WARN("Test warning message"));
    EXPECT_NO_THROW(VEST_CORE_ERROR("Test error message"));
}

TEST_F(LogTests, ClientLoggerOutput) {
    EXPECT_NO_THROW(VEST_INFO("Client info message"));
    EXPECT_NO_THROW(VEST_WARN("Client warning message"));
    EXPECT_NO_THROW(VEST_ERROR("Client error message"));
}

TEST_F(LogTests, LogFileCreation) {
    VEST_CORE_INFO("Testing file creation");
    
    // Force flush
    Log::GetCoreLogger()->flush();
    
    // Check if log file was created
    EXPECT_TRUE(std::filesystem::exists("VestEngine.log"));
}

TEST_F(LogTests, FormattedLogging) {
    const int value = 42;
    const std::string text = "test";
    
    EXPECT_NO_THROW(VEST_CORE_INFO("Value: {0}, Text: {1}", value, text));
    EXPECT_NO_THROW(VEST_INFO("Formatted: {0} + {1} = {2}", 1, 2, 3));
}

TEST_F(LogTests, TraceLogging) {
    EXPECT_NO_THROW(VEST_CORE_TRACE("Trace message"));
    EXPECT_NO_THROW(VEST_TRACE("Client trace message"));
}

}  // namespace Vest
