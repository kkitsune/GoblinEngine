add_subdirectory(libs)

add_library(engine SHARED
		include/engine/Engine.hpp
		source/Engine.cpp)

target_include_directories(engine PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:include>
		PRIVATE source)

export(TARGETS engine FILE EngineConfig.cmake)

target_compile_options(engine
		PRIVATE ${DEFAULT_COMPILE_OPTIONS})

target_compile_definitions(engine
		PRIVATE ${DEFAULT_COMPILE_DEFINITIONS} E_ABI_engine
		PUBLIC GLM_FORCE_RADIANS)

target_link_libraries(engine ${DEFAULT_LINKER_OPTIONS} ${SDL2_LIBRARIES} bgfx
		PUBLIC glm spdlog type_safe ctti tweeny nlohmann_json)

install(TARGETS engine
		ARCHIVE  DESTINATION lib
		LIBRARY  DESTINATION lib
		RUNTIME  DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
