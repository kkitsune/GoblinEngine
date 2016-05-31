project(engine VERSION 0.1 LANGUAGES CXX)

add_library(engine SHARED
		include/ctti/detail/pretty_function.hpp
		include/ctti/detail/string.hpp
		include/ctti/detail/array.hpp
		include/ctti/detail/hash.hpp
		include/ctti/type_id.hpp

		include/KHR/khrplatform.h
		include/glad/glad.h
		source/deps/glad/glad.c

		include/doctest/doctest.h

		source/deps/nanovg/nanovg_gl_utils.h
		source/deps/nanovg/stb_truetype.h
		source/deps/nanovg/nanovg_gl.h
		source/deps/nanovg/stb_image.h
		source/deps/nanovg/fontstash.h
		source/deps/nanovg/nanovg.h
		source/deps/nanovg/nanovg.c
		source/deps/nanovg/blender_icons.h
		source/deps/nanovg/blendish.h

		include/engine/utils/imconfig.h
		include/engine/utils/imgui.h
		source/deps/imgui/imgui_impl_glfw_gl3.cpp
		source/deps/imgui/imgui_impl_glfw_gl3.h
		source/deps/imgui/imgui_internal.h
		source/deps/imgui/stb_rect_pack.h
		source/deps/imgui/stb_textedit.h
		source/deps/imgui/stb_truetype.h
		source/deps/imgui/imgui_demo.cpp
		source/deps/imgui/imgui_draw.cpp
		source/deps/imgui/imgui.cpp

		include/engine/utils/BitmaskOperators.hpp
		include/engine/utils/Json.hpp
		include/engine/utils/Sol.hpp

		include/engine/scene/Node.hpp
		source/scene/Node.cpp

		include/engine/resource/VertexArray.hpp
		include/engine/resource/Framebuffer.hpp
		include/engine/resource/Texture.hpp
		include/engine/resource/Sampler.hpp
		include/engine/resource/Buffer.hpp
		include/engine/resource/Shader.hpp
		include/engine/resource/Handle.hpp
		include/engine/Application.hpp
		include/engine/InputEnums.hpp
		include/engine/Blendish.hpp
		include/engine/Painter.hpp
		include/engine/Engine.hpp
		include/engine/Time.hpp

		source/resource/Texture.cpp
		source/resource/Shader.cpp
		source/Application.cpp
		source/Blendish.cpp
		source/Painter.cpp
		source/Engine.cpp)

target_include_directories(engine PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:include>
		PRIVATE source)

export(TARGETS engine FILE EngineConfig.cmake)
generate_export_header(engine
		EXPORT_FILE_NAME  include/engine/config.h
		EXPORT_MACRO_NAME ENGINE_API)

target_compile_options(engine
		PRIVATE ${DEFAULT_COMPILE_OPTIONS})

target_compile_definitions(engine
		PRIVATE ${DEFAULT_COMPILE_DEFINITIONS} engine_EXPORTS GLAD_GLAPI_EXPORT_BUILD
		PUBLIC GLAD_GLAPI_EXPORT GLM_FORCE_RADIANS GLM_FORCE_SSE2)

pkg_search_module(GLFW REQUIRED glfw3)
find_package(OpenGL REQUIRED)
find_package(Lua REQUIRED)

target_link_libraries(engine ${DEFAULT_LINKER_OPTIONS}
		Poco::Foundation Poco::Net ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${LUA_LIBRARIES})

install(TARGETS engine
		ARCHIVE  DESTINATION lib
		LIBRARY  DESTINATION lib
		RUNTIME  DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
