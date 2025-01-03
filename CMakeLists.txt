cmake_minimum_required(VERSION 3.9)
project(shooter)

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_HEADERS
        main.h
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
        src/objects/backgrounds/Ground.h
        src/objects/interactive/Crystal.h
        src/objects/interactive/Heart.h
        src/objects/interactive/Ammunition.h
        src/objects/interactive/Platform.h
        src/objects/units/Unit.h
        src/objects/units/Shooter.h
        src/objects/interactive/Bullet.h
        src/objects/interfaces/Movable.h
        src/ui/Button.h
        src/ui/Text.h
        include/tinyxml/tinyxml2.h
)
set(PROJECT_SOURCES
        main.cpp 
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
        src/objects/backgrounds/Ground.cpp
        src/objects/interactive/Crystal.cpp
        src/objects/interactive/Heart.cpp
        src/objects/interactive/Ammunition.cpp
        src/objects/interactive/Platform.cpp
        src/objects/units/Unit.cpp
        src/objects/units/Shooter.cpp
        src/objects/interactive/Bullet.cpp
        src/objects/interfaces/Movable.cpp
        src/ui/Button.cpp
        src/ui/Text.cpp
        include/tinyxml/tinyxml2.cpp
)

add_executable(shooter ${PROJECT_SOURCES} ${PROJECT_HEADERS})