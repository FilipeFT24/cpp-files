cmake_minimum_required(VERSION 3.23)

#include(cmake/prelude.cmake)

project(headeronly)

#include(cmake/project-is-top-level.cmake)
#include(cmake/variables.cmake)

# ---- Declare library ----

add_library(headeronly INTERFACE)

target_include_directories(
        headeronly ${warning_guard}
        INTERFACE
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(headeronly INTERFACE cxx_std_20)

# ---- Install rules ----

# ---- Developer mode ----

add_subdirectory(test)