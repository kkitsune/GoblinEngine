cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(GoblinEngine)

set(CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_STANDARD 14)

include(GenerateExportHeader)
include(ExternalProject)
include(WriteCompilerDetectionHeader)

include(compile-options.cmake)

# Set policies
cmake_policy(SET CMP0028 NEW) # ENABLE CMP0028: Double colon in target name means ALIAS or IMPORTED target.
cmake_policy(SET CMP0054 NEW) # ENABLE CMP0054: Only interpret if() arguments as variables or keywords when unquoted.
cmake_policy(SET CMP0042 NEW) # ENABLE CMP0042: MACOSX_RPATH is enabled by default.
cmake_policy(SET CMP0063 NEW) # ENABLE CMP0063: Honor visibility properties for all target types.

# Set runtime path
set(CMAKE_SKIP_BUILD_RPATH            FALSE) # Add absolute path to all dependencies for BUILD
set(CMAKE_BUILD_WITH_INSTALL_RPATH    FALSE) # Use CMAKE_INSTALL_RPATH for INSTALL
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE) # Do NOT add path to dependencies for INSTALL

find_package(PkgConfig REQUIRED)
find_package(Poco REQUIRED COMPONENTS Foundation Net)

enable_testing()
add_subdirectory(engine)
add_subdirectory(game)
