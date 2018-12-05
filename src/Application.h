#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Scene.h"
#include "Event.h"
#include "Window.h"

class Event;
class Scene;
class Window;
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
