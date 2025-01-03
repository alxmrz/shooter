#pragma once

#include <SFML/Graphics.hpp>

class EventHandler;
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
    EventHandler* event;
    
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
    sf::Time timePerFrame = sf::seconds(1.f/60.f);
};
