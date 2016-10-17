cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(GoblinEngine)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

include(GenerateExportHeader)
include(ExternalProject)

include(compile-options.cmake)

set(CMAKE_SKIP_BUILD_RPATH            FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH    FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

cmake_policy(SET CMP0004 OLD)

add_subdirectory(engine)
add_subdirectory(example)
