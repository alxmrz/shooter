#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Scene.h"
#include "Event.h"

class Event;
class Scene;

class Application
{
public:
    Application();
    ~Application();
    
    sf::RenderWindow* window;
    Event* event;
    Scene* scene;
    
    int run();
private:
    GameState* gameState;
    
    void initWindow();
    void draw();
    
};
