cmake_minimum_required(VERSION 3.27.4)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

add_compile_options(-Wfatal-errors -w -O2 -fPIC -g)

project(game)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME}
    external/SFMLAnimation/src/AnimationManager.cpp
    game/src/main.cpp
    game/src/game.cpp
    game/src/map.cpp
    game/src/entity.cpp
    game/src/character.cpp
    game/src/player.cpp
    game/src/camera.cpp
    game/src/ui.cpp
    )

include_directories(".")

target_include_directories(${PROJECT_NAME}
    PUBLIC /usr/include/SFML
    PUBLIC external
    PUBLIC game/include
    PUBLIC external/glm
    PUBLIC external/SFMLAnimation/src
    PUBLIC external/SSBMTonberry/tileson
    )

target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    )

target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    )

    #set_target_properties(${PROJECT_NAME}
    #    PROPERTIES SUFFIX ".html"
    #    LINK_FLAGS " --bind --emrun \
    #    -s FORCE_FILESYSTEM \
    #    -s FULL_ES3=1 \
    #    -s EXPORT_ALL=1 \
    #    -s MALLOC=emmalloc \
    #    -s WASM=1 \
    #    -s ALLOW_MEMORY_GROWTH=1 \
    #    -Wemcc \
    #    -s MIN_WEBGL_VERSION=2 \
    #    -s USE_LIBPNG=1 \
    #    -s USE_WEBGL2=1 \
    #    -s USE_GLFW=3 \
    #    --preload-file ../../res \
    #    --preload-file ../../shaders")
