
if(NOT OPTION_BUILD_EXAMPLES)
    return()
endif()

# 
# External dependencies
# 

# find_package(THIRDPARTY REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)

# 
# Executable name and options
# 

# Target name
set(target example)

# Exit here if required dependencies are not met
message(STATUS "Example ${target}")


# 
# Sources
# 

set(sources
    main.cpp
)


# 
# Create executable
# 

# Build executable
add_executable(${target}
    MACOSX_BUNDLE WIN32
    ${sources}
)

# Create namespaced alias
add_executable(${META_PROJECT_NAME}::${target} ALIAS ${target})


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
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${PROJECT_BINARY_DIR}/source/include
    ${glbinding_output}/include
    ${anax_output}/include
)


# 
# Libraries
#

add_dependencies(${target} glbinding anax glfw)

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    ${Boost_LIBRARIES}
    ${META_PROJECT_NAME}::engine
    ${anax_output}/lib/libanax.a
    ${glbinding_output}/lib/libglbinding.dll.a
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
    -DGLM_FORCE_RADIANS -DGLM_FORCE_SSE2
)


# 
# Compile options
#

target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)


# 
# Linker options
#

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
)


# 
# Deployment
# 

# Executable
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT examples
    BUNDLE  DESTINATION ${INSTALL_BIN} COMPONENT examples
)
