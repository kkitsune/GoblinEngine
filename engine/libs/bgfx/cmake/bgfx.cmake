# bgfx.cmake - bgfx building in cmake
# Written in 2016 by Joshua Brookover <josh@jalb.me>

# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty.

# You should have received a copy of the CC0 Public Domain Dedication along with
# this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

# Ensure the directory exists
if( NOT IS_DIRECTORY ${BGFX_DIR} )
	message( SEND_ERROR "Could not load bgfx, directory does not exist. ${BGFX_DIR}" )
	return()
endif()

# Grab the bgfx source files
file( GLOB BGFX_SOURCES ${BGFX_DIR}/src/*.cpp ${BGFX_DIR}/src/*.mm ${BGFX_DIR}/src/*.h ${BGFX_DIR}/include/bgfx/*.h ${BGFX_DIR}/include/bgfx/c99/*.h )

# Create the bgfx target
add_library( bgfx STATIC ${BGFX_SOURCES} )

# Enable BGFX_CONFIG_DEBUG in Debug configuration
target_compile_definitions( bgfx PRIVATE "$<$<CONFIG:Debug>:BGFX_CONFIG_DEBUG=1>" )

# Special Visual Studio Flags
if( MSVC )
	target_compile_definitions( bgfx PRIVATE "_CRT_SECURE_NO_WARNINGS" )
endif()

# Includes
target_include_directories( bgfx PRIVATE ${BGFX_DIR}/3rdparty ${BGFX_DIR}/3rdparty/dxsdk/include ${BGFX_DIR}/3rdparty/khronos )
target_include_directories( bgfx PUBLIC ${BGFX_DIR}/include )

find_package( OpenGL REQUIRED )

# bgfx depends on bx
target_link_libraries( bgfx PUBLIC bx ${OPENGL_LIBRARIES} )

# ovr support
if( BGFX_USE_OVR )
	target_link_libraries( bgfx PUBLIC ovr )
endif()

if( UNIX AND NOT APPLE )
	target_link_libraries( bgfx PUBLIC X11 pthread dl )
endif()

# Link against psapi in Visual Studio
if( MSVC )
	target_link_libraries( bgfx PUBLIC psapi )
endif()

# Frameworks required on OS X
if( APPLE )
	find_library( COCOA_LIBRARY Cocoa )
	find_library( METAL_LIBRARY Metal )
	find_library( QUARTZCORE_LIBRARY QuartzCore )
	mark_as_advanced( COCOA_LIBRARY )
	mark_as_advanced( METAL_LIBRARY )
	mark_as_advanced( QUARTZCORE_LIBRARY )
	target_link_libraries( bgfx PUBLIC ${COCOA_LIBRARY} ${METAL_LIBRARY} ${QUARTZCORE_LIBRARY} )
endif()

# Excluded files from compilation
set_source_files_properties( ${BGFX_DIR}/src/amalgamated.cpp PROPERTIES HEADER_FILE_ONLY ON )
set_source_files_properties( ${BGFX_DIR}/src/amalgamated.mm PROPERTIES HEADER_FILE_ONLY ON )
set_source_files_properties( ${BGFX_DIR}/src/glcontext_ppapi.cpp PROPERTIES HEADER_FILE_ONLY ON )
set_source_files_properties( ${BGFX_DIR}/src/glcontext_egl.cpp PROPERTIES HEADER_FILE_ONLY ON )

if( APPLE OR MSVC )
	set_source_files_properties( ${BGFX_DIR}/src/glcontext_glx.cpp PROPERTIES HEADER_FILE_ONLY ON )
endif()

# Exclude mm files if not on OS X
if( NOT APPLE )
	set_source_files_properties( ${BGFX_DIR}/src/glcontext_eagl.mm PROPERTIES HEADER_FILE_ONLY ON )
	set_source_files_properties( ${BGFX_DIR}/src/glcontext_nsgl.mm PROPERTIES HEADER_FILE_ONLY ON )
	set_source_files_properties( ${BGFX_DIR}/src/renderer_mtl.mm PROPERTIES HEADER_FILE_ONLY ON )
endif()

# Put in a "bgfx" folder in Visual Studio
set_target_properties( bgfx PROPERTIES FOLDER "bgfx" )

# Export debug build as "bgfxd"
set_target_properties( bgfx PROPERTIES OUTPUT_NAME_DEBUG "bgfxd" )
