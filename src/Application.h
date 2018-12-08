#pragma once

#include <SFML/Graphics.hpp>
#include "Window.h"
#include "Scene.h"
#include "Event.h"
#include "GameState.h"

class Event;
class Scene;
class Window;
class GameState;

class Application
{
public:
    Application();
    ~Application();
    
    Window* window;
    Event* event;
    Scene* scene;
    
    int run();
private:
    int fps = 120;
    GameState* gameState;
};
