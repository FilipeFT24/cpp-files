#project(headeronlyTests LANGUAGES CXX)

#include(../cmake/project-is-top-level.cmake)
#include(../cmake/folders.cmake)

# ---- Dependencies ----

#if(PROJECT_IS_TOP_LEVEL)
#    find_package(headeronly REQUIRED)
    enable_testing()
#endif()

# ---- Tests ----

find_package(GTest CONFIG REQUIRED)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include(GoogleTest)

add_executable(headeronly_test test.cpp)
target_link_libraries(headeronly_test PRIVATE headeronly GTest::gtest_main)
#target_compile_features(headeronly_test cxx_std_20)

gtest_discover_tests(headeronly_test)

#add_test(NAME headeronly_test COMMAND headeronly_test)