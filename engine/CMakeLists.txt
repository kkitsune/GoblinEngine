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
		include/engine/utils/Mustache.hpp
		include/engine/utils/Json.hpp
		include/engine/utils/Sol.hpp

		include/ponder/detail/variant_recursive_wrapper.hpp
		include/ponder/detail/simplepropertyimpl.hpp
		include/ponder/detail/simplepropertyimpl.inl
		include/ponder/detail/arraypropertyimpl.hpp
		include/ponder/detail/arraypropertyimpl.inl
		include/ponder/detail/enumpropertyimpl.hpp
		include/ponder/detail/enumpropertyimpl.inl
		include/ponder/detail/userpropertyimpl.hpp
		include/ponder/detail/userpropertyimpl.inl
		include/ponder/detail/observernotifier.hpp
		include/ponder/detail/constructorimpl.hpp
		include/ponder/detail/propertyfactory.hpp
		include/ponder/detail/functiontraits.hpp
		include/ponder/detail/issmartpointer.hpp
		include/ponder/detail/valueprovider.hpp
		include/ponder/detail/classmanager.hpp
		include/ponder/detail/functionimpl.hpp
		include/ponder/detail/objectholder.hpp
		include/ponder/detail/objectholder.inl
		include/ponder/detail/objecttraits.hpp
		include/ponder/detail/enummanager.hpp
		include/ponder/detail/returntype.hpp
		include/ponder/detail/dictionary.hpp
		include/ponder/detail/valueimpl.hpp
		include/ponder/detail/yesnotype.hpp
		include/ponder/detail/yesnotype.hpp
		include/ponder/detail/variant.hpp
		include/ponder/detail/rawtype.hpp
		include/ponder/detail/getter.hpp
		include/ponder/detail/getter.inl
		include/ponder/detail/typeid.hpp
		include/ponder/detail/util.hpp
		include/ponder/simpleproperty.hpp
		include/ponder/arrayproperty.hpp
		include/ponder/classvisitor.hpp
		include/ponder/valuevisitor.hpp
		include/ponder/userproperty.hpp
		include/ponder/classbuilder.hpp
		include/ponder/classbuilder.inl
		include/ponder/enumproperty.hpp
		include/ponder/constructor.hpp
		include/ponder/arraymapper.hpp
		include/ponder/valuemapper.hpp
		include/ponder/enumbuilder.hpp
		include/ponder/pondertype.hpp
		include/ponder/enumobject.hpp
		include/ponder/enumobject.inl
		include/ponder/userobject.hpp
		include/ponder/userobject.inl
		include/ponder/tagholder.hpp
		include/ponder/classcast.hpp
		include/ponder/classget.hpp
		include/ponder/classget.inl
		include/ponder/function.hpp
		include/ponder/observer.hpp
		include/ponder/property.hpp
		include/ponder/enumget.hpp
		include/ponder/enumget.inl
		include/ponder/config.hpp
		include/ponder/errors.hpp
		include/ponder/error.hpp
		include/ponder/error.inl
		include/ponder/value.inl
		include/ponder/value.hpp
		include/ponder/class.hpp
		include/ponder/class.inl
		include/ponder/type.hpp
		include/ponder/args.hpp
		include/ponder/enum.hpp
		include/ponder/enum.inl
		include/ponder/format.h
		source/deps/ponder/observernotifier.cpp
		source/deps/ponder/simpleproperty.cpp
		source/deps/ponder/arrayproperty.cpp
		source/deps/ponder/classmanager.cpp
		source/deps/ponder/classvisitor.cpp
		source/deps/ponder/userproperty.cpp
		source/deps/ponder/enumproperty.cpp
		source/deps/ponder/enumbuilder.cpp
		source/deps/ponder/enummanager.cpp
		source/deps/ponder/userobject.cpp
		source/deps/ponder/pondertype.cpp
		source/deps/ponder/enumobject.cpp
		source/deps/ponder/tagholder.cpp
		source/deps/ponder/classcast.cpp
		source/deps/ponder/function.cpp
		source/deps/ponder/property.cpp
		source/deps/ponder/observer.cpp
		source/deps/ponder/format.cpp
		source/deps/ponder/errors.cpp
		source/deps/ponder/class.cpp
		source/deps/ponder/value.cpp
		source/deps/ponder/error.cpp
		source/deps/ponder/args.cpp
		source/deps/ponder/util.cpp
		source/deps/ponder/enum.cpp

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
		PRIVATE ${DEFAULT_COMPILE_DEFINITIONS} engine_EXPORTS GLAD_GLAPI_EXPORT_BUILD PONDER_EXPORTS
		PUBLIC GLAD_GLAPI_EXPORT GLM_FORCE_RADIANS GLM_FORCE_SSE2)

pkg_search_module(GLFW REQUIRED glfw3)
find_package(OpenGL REQUIRED)
find_package(Lua REQUIRED)

target_link_libraries(engine ${DEFAULT_LINKER_OPTIONS}
		${Boost_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${LUA_LIBRARIES})

install(TARGETS engine
		ARCHIVE  DESTINATION lib
		LIBRARY  DESTINATION lib
		RUNTIME  DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
