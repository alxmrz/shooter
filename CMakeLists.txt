cmake_minimum_required(VERSION 3.9)
project(shooter)

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_HEADERS
        src/Application.h
        src/CObject.h
        src/Event.h
        src/EventHandler.h
        src/GameObjects.h
        src/GameState.h
        src/Map.h
        src/AI.h
        src/Scene.h
        src/Window.h
        src/Fabric.h
        src/Resources.h
        src/LevelGenerator.h
        src/SpacingGrid.h
        src/StageProgress.h
        src/objects/backgrounds/Ground.h
        src/objects/backgrounds/GroundBlock.h
        src/objects/backgrounds/GroundBlock.h
    src/objects/interactive/Crystal.h
        src/objects/interactive/Heart.h
        src/objects/interactive/Ammunition.h
        src/objects/interactive/Platform.h
        src/objects/interactive/Tree.h
        src/objects/interactive/Flag.h
        src/objects/units/Unit.h
        src/objects/units/Shooter.h
        src/objects/interactive/Bullet.h
        src/objects/interactive/Aim.h
        src/objects/interactive/ExplosionFrames.h
        src/objects/interfaces/Movable.h
        src/ui/Button.h
        src/ui/Text.h
        src/ui/Dialogue.h
        include/tinyxml/tinyxml2.h
)
set(PROJECT_SOURCES
        src/Application.cpp
        src/CObject.cpp 
        src/Event.cpp
        src/EventHandler.cpp
        src/GameObjects.cpp
        src/GameState.cpp
        src/Map.cpp
        src/AI.cpp
        src/Scene.cpp
        src/Window.cpp
        src/Fabric.cpp
        src/Resources.cpp
        src/LevelGenerator.cpp
        src/StageProgress.cpp
        src/objects/backgrounds/Ground.cpp
        src/objects/backgrounds/GroundBlock.cpp
        src/objects/backgrounds/GroundBlock.cpp
    src/objects/interactive/Crystal.cpp
        src/objects/interactive/Heart.cpp
        src/objects/interactive/Ammunition.cpp
        src/objects/interactive/Platform.cpp
        src/objects/interactive/Tree.cpp
        src/objects/interactive/Flag.cpp
        src/objects/units/Unit.cpp
        src/objects/units/Shooter.cpp
        src/objects/interactive/Bullet.cpp
        src/objects/interactive/Aim.cpp
        src/objects/interfaces/Movable.cpp
        src/ui/Button.cpp
        src/ui/Text.cpp
        src/ui/Dialogue.cpp
        include/tinyxml/tinyxml2.cpp
)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_library(shooter_lib STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})

add_executable(shooter main.cpp)
target_link_libraries(shooter shooter_lib sfml-graphics sfml-window sfml-system sfml-audio)

# --- Testing ---
option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    add_subdirectory(tests)
endif()
