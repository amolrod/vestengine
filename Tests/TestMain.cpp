#include <gtest/gtest.h>
#include "Core/Log.h"

// Main test runner
// Individual tests are in separate files
int main(int argc, char** argv) {
    // Initialize logging system before any tests
    Vest::Log::Init();
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
