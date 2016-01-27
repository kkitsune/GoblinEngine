
# 
# External dependencies
# 

# find_package(THIRDPARTY REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)

# 
# Library name and options
# 

# Target name
set(target engine)

# Exit here if required dependencies are not met
message(STATUS "Lib ${target}")

# Set API export file and macro
string(TOUPPER ${target} target_upper)
set(feature_file "include/${target}/${target}_features.h")
set(export_file  "include/${target}/${target}_api.h")
set(export_macro "${target_upper}_API")


# 
# Sources
# 

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(headers
    ${include_path}/baselib.h
)

set(sources
    ${source_path}/baselib.cpp
)

# NanoVG from https://github.com/memononen/nanovg (with some modifications)
set(sources ${sources}
    ${source_path}/nanovg/fontstash.h
    ${source_path}/nanovg/nanovg.c
    ${source_path}/nanovg/nanovg.h
    ${source_path}/nanovg/nanovg_gl.h
    ${source_path}/nanovg/nanovg_gl_utils.h
    ${source_path}/nanovg/stb_image.h
    ${source_path}/nanovg/stb_truetype.h
)

# Gamepad from https://github.com/elanthis/gamepad (does not support MacOS)
set(GAMEPAD_LIB)
set(NATIVE_DEFINITIONS)

if(UNIX AND NOT APPLE)
    set(NATIVE_DEFINITIONS -DGLFW_EXPOSE_NATIVE_X11)
    set(GAMEPAD_LIB udev)
    set(sources ${sources}
        ${source_path}/gamepad/gamepad.c
        ${source_path}/gamepad/gamepad.h
    )
endif()
if(APPLE)
    set(NATIVE_DEFINITIONS -DGLFW_EXPOSE_NATIVE_COCOA -DGLFW_EXPOSE_NATIVE_NSGL -DGOBLIN_NO_GAMEPAD)
endif()
if(WIN32)
    set(NATIVE_DEFINITIONS -DGLFW_EXPOSE_NATIVE_WIN32 -DGLFW_EXPOSE_NATIVE_WGL)
    set(GAMEPAD_LIB XInput)
    set(sources ${sources}
        ${source_path}/gamepad/gamepad.c
        ${source_path}/gamepad/gamepad.h
    )
endif()

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${headers})
source_group_by_path(${source_path}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})


# 
# Create library
# 

# Build library
add_library(${target}
    ${sources}
    ${headers}
)

# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})

# Export library for downstream projects
export(TARGETS ${target} NAMESPACE ${META_PROJECT_NAME}:: FILE ${PROJECT_BINARY_DIR}/cmake/${target}/${target}-export.cmake)

# Create feature detection header
# Compilers: https://cmake.org/cmake/help/v3.1/variable/CMAKE_LANG_COMPILER_ID.html#variable:CMAKE_%3CLANG%3E_COMPILER_ID
# Feature: https://cmake.org/cmake/help/v3.1/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html
write_compiler_detection_header(
    FILE ${feature_file}
    PREFIX ${target_upper}
    COMPILERS AppleClang Clang GNU MSVC
    FEATURES cxx_alignas cxx_alignof cxx_constexpr cxx_final cxx_noexcept cxx_nullptr cxx_sizeof_member cxx_thread_local
)

# Create API export header
generate_export_header(${target}
    EXPORT_FILE_NAME  ${export_file}
    EXPORT_MACRO_NAME ${export_macro}
)


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${PROJECT_BINARY_DIR}/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${glbinding_output}/include
    ${anax_output}/include
    ${glfw_output}/include

    PUBLIC
    ${DEFAULT_INCLUDE_DIRECTORIES}

    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


# 
# Libraries
#

add_dependencies(${target} glbinding anax glfw)

target_link_libraries(${target}
    PRIVATE
    ${GAMEPAD_LIB}
    ${anax_output}/lib/libanax.a
    ${glfw_output}/lib/glfw3dll.a

    PUBLIC
    ${DEFAULT_LIBRARIES}
    ${Boost_LIBRARIES}
    ${glbinding_output}/lib/libglbinding.dll.a

    INTERFACE
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE
    -DGAMEPAD_STATIC_LIB
    ${NATIVE_DEFINITIONS}

    PUBLIC
    $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${target_upper}_STATIC_DEFINE>
    ${DEFAULT_COMPILE_DEFINITIONS}
    -DGLM_FORCE_RADIANS -DGLM_FORCE_SSE2 -DGLFW_INCLUDE_NONE

    INTERFACE
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_COMPILE_OPTIONS}

    INTERFACE
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}

    INTERFACE
)


# 
# Deployment
# 

# Library
install(TARGETS ${target}
    EXPORT  "${target}-export"            COMPONENT dev
    RUNTIME DESTINATION ${INSTALL_BIN}    COMPONENT runtime
    LIBRARY DESTINATION ${INSTALL_SHARED} COMPONENT runtime
    ARCHIVE DESTINATION ${INSTALL_LIB}    COMPONENT dev
)

# Header files
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE}
    COMPONENT dev
)

# Generated header files
install(DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE}
    COMPONENT dev
)

# CMake config
install(EXPORT ${target}-export
    NAMESPACE   ${META_PROJECT_NAME}::
    DESTINATION ${INSTALL_CMAKE}/${target}
    COMPONENT   dev
)
