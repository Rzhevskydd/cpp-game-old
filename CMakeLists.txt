cmake_minimum_required(VERSION 3.12)
project(game
    LANGUAGES CXX)

if (MSVC)
    add_compile_options(/W4 /WX)
    add_link_options(/WX)
else()
    add_compile_options(
        -Wall
        -Wextra
        -Werror
        -pedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
#        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
    )
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(game
    src/main.cpp
    src/Animation
    src/Command.cpp
    src/CommandQueue.cpp
    src/DataTables.cpp
    src/Entity.cpp
    src/Game.cpp
    src/Person.cpp
    src/SceneNode.cpp
    src/TextNode.cpp
    src/User.cpp
    src/Utility.cpp
    src/World.cpp
    src/State.cpp
    src/StateStack.cpp
    src/MenuState.cpp
    src/TitleState.cpp
    src/GameState.cpp
    src/PauseState.cpp
    src/State.cpp
    src/StateStack.cpp
    src/Container.cpp
    src/Component.cpp
    src/Application.cpp
    src/Button.cpp
    src/Pickup.cpp
    src/Projectile.cpp
    src/LevelManager.cpp
    src/LevelObject.cpp
    src/TileLayout.cpp
    src/LevelObject.cpp
    src/BoundingBox.cpp
    src/DeathState.cpp
    src/bot.cpp
    src/bot/classai.cpp
    src/bot/layers.cpp
    src/bot/matrix.cpp
    src/bot/networks.cpp
    src/ForegroundManager.cpp
)


set(CMAKE_CXX_COMPILER_FRONTEND_VARIANT GNU)
set_target_properties(game PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_link_libraries(game ${CONAN_LIBS})
target_include_directories(game PRIVATE include)
target_include_directories(game SYSTEM PUBLIC ${CONAN_INCLUDE_DIRS})

if(BRD_WANT_ASAN AND UNIX AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    target_compile_options(game PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
    )
    target_link_options(game PRIVATE
        -fsanitize=address
    )
endif()

# clangd stuff
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
