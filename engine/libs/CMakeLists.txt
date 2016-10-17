find_package(SDL2 REQUIRED)

add_subdirectory(type_safe)
add_subdirectory(tweeny)
add_subdirectory(spdlog)
add_subdirectory(bgfx)
add_subdirectory(json)
add_subdirectory(glm)

add_library(ctti INTERFACE)
target_include_directories(ctti INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ctti/include)
