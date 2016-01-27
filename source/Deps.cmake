
ExternalProject_Add(glbinding
        GIT_REPOSITORY https://github.com/cginternals/glbinding.git
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glbinding
        CMAKE_CACHE_ARGS -DOPTION_BUILD_TESTS:bool=OFF -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        )

set(glbinding_output ${CMAKE_CURRENT_BINARY_DIR}/glbinding)

ExternalProject_Add(anax
        GIT_REPOSITORY https://github.com/miguelmartin75/anax.git
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/anax
        CMAKE_CACHE_ARGS -DBUILD_SHARED_LIBS:bool=OFF -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        )

set(anax_output ${CMAKE_CURRENT_BINARY_DIR}/anax)

ExternalProject_Add(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG tags/3.1.2
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glfw
        CMAKE_CACHE_ARGS -DBUILD_SHARED_LIBS:bool=ON -DGLFW_BUILD_EXAMPLES:bool=OFF -DGLFW_BUILD_TESTS:bool=OFF -DGLFW_BUILD_DOCS:bool=OFF -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        )

set(glfw_output ${CMAKE_CURRENT_BINARY_DIR}/glfw)
