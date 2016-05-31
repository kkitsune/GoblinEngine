project(game VERSION 0.1 LANGUAGES CXX)

add_executable(game MACOSX_BUNDLE
		main.cpp)

target_link_libraries(game ${DEFAULT_LINKER_OPTIONS}
		${Boost_LIBRARIES} ${OPENGL_LIBRARIES} ${LUA_LIBRARIES} engine)

target_compile_options(game
		PRIVATE ${DEFAULT_COMPILE_OPTIONS})

target_compile_definitions(game
		PRIVATE ${DEFAULT_COMPILE_DEFINITIONS})
