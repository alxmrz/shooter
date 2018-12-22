#pragma once

#include <SFML/Graphics.hpp>

class Event;
class Scene;
class Window;
class GameState;

class Application {
public:
    Application();
    ~Application();

    /**
     * @var class to display all
     */
    Window* window;
    
    /**
     * @var class for handling events
     */
    Event* event;
    
    /**
     * class for creating scene on the window
     */
    Scene* scene;

    /**
     * Entry point of the application
     * 
     * @return execution status 
     */
    int run();
    void close();
private:
    /**
     * @var current game state
     */
    GameState* gameState;
};
