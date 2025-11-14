# VestEngine Tests
cmake_minimum_required(VERSION 3.20)

# Test executable
add_executable(VestTests
    TestMain.cpp
    Core/LogTests.cpp
    Serialization/SceneSerializerTests.cpp
    Commands/CommandTests.cpp
)

target_link_libraries(VestTests
    PRIVATE
    VestEngine
    GTest::gtest
    GTest::gtest_main
)

target_include_directories(VestTests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/VestEngine/src
    ${CMAKE_SOURCE_DIR}/Editor/src
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(VestTests)
