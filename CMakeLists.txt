cmake_minimum_required(VERSION 3.0)
project(cubel-all)

if (CMAKE_BUILD_TYPE MATCHES debug-wasm)
    add_subdirectory("${PROJECT_SOURCE_DIR}/project-wasm" "${PROJECT_SOURCE_DIR}/build-wasm")
elseif(CMAKE_BUILD_TYPE MATCHES release)
    add_subdirectory("${PROJECT_SOURCE_DIR}/project-wasm" "${PROJECT_SOURCE_DIR}/release")
else()
    add_subdirectory("${PROJECT_SOURCE_DIR}/project-stand" "${PROJECT_SOURCE_DIR}/build-stand")
endif()