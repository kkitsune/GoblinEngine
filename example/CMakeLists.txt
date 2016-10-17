add_executable(example MACOSX_BUNDLE
		example.cpp)

find_package(SDL2 REQUIRED)
target_link_libraries(example ${DEFAULT_LINKER_OPTIONS} ${SDL2_LIBRARIES} engine)

target_compile_options(example
		PRIVATE ${DEFAULT_COMPILE_OPTIONS})

target_compile_definitions(example
		PRIVATE ${DEFAULT_COMPILE_DEFINITIONS})
