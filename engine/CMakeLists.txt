add_subdirectory(libs)

add_library(engine STATIC include/ABI.h
		include/engine/InputEnums.hpp
		include/engine/Serializable.hpp
		include/engine/Engine.hpp
		include/engine/Math.hpp
		include/engine/Node.hpp
		source/Engine.cpp
		source/Node.cpp)

target_include_directories(engine PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:include>
		PRIVATE source)

target_compile_options(engine
		PRIVATE ${DEFAULT_COMPILE_OPTIONS})

target_compile_definitions(engine
		PRIVATE ${DEFAULT_COMPILE_DEFINITIONS} E_ABI_engine E_ABI_STATIC_engine
		PUBLIC GLM_FORCE_RADIANS)

target_link_libraries(engine PUBLIC ${DEFAULT_LINKER_OPTIONS} ${SDL2_LIBRARIES}
		glm bgfx spdlog type_safe ctti tweeny nlohmann_json)

install(TARGETS engine
		ARCHIVE  DESTINATION lib
		LIBRARY  DESTINATION lib
		RUNTIME  DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
