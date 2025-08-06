cmake_minimum_required(VERSION 3.27.4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

add_compile_options(-Werror -w -g -fPIC -D_DEBUG -fdiagnostics-color=always)

project(game)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME}
    external/SFMLAnimation/src/AnimationManager.cpp
    external/glad/src/glad.c
    external/stb/stb_image.c
    game/src/game.cpp
    game/src/gl_error_manager.cpp
    game/src/main.cpp
    game/src/resource_manager.cpp
    game/src/texture.cpp
    game/src/shader.cpp
    game/src/sprite.cpp
    game/src/renderer.cpp
    game/src/sprite_renderer.cpp
    game/src/map.cpp
    game/src/entity.cpp
    game/src/character.cpp
    game/src/player.cpp
    game/src/camera.cpp
    game/src/ui.cpp
    game/src/button.cpp
    game/src/label.cpp
    game/src/npc.cpp
    game/src/time_manager.cpp
    game/src/collision.cpp
    game/src/building.cpp
    game/src/scene.cpp
    )

include_directories(".")

target_include_directories(${PROJECT_NAME}
    PUBLIC /usr/include/SFML
    #PUBLIC /home/david/Documents/GameDev/tools/SFML/include
    PUBLIC external
    PUBLIC game/include
    PUBLIC external/glad/include
    PUBLIC external/glm
    PUBLIC external/SFMLAnimation/src
    PUBLIC external/SSBMTonberry/tileson
    PUBLIC external/SFMLGlCheck
    PUBLIC external/stb
    )


find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)

target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    #PRIVATE /home/david/Documents/GameDev/tools/SFML/src
    )

target_link_libraries(${PROJECT_NAME}
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    )
