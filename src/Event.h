#ifndef EVENT_H
#define EVENT_H

#include <SFML/Graphics.hpp>

class Application;
class GameState;

class Event {
public:
    Event();
    Event(Application* app, GameState* gameState);
    virtual ~Event();
    
    /**
     * Handle all events
     */
    void handle();
private:
    Application* app;
    GameState* gameState;

    /**
     * Handle events of usual keys
     * 
     * @param event
     */
    void handleKeys(sf::Event* event);
    
    /**
     * Handle arrow keys events
     * 
     * @param event
     */
    void handelArrowKeys();
    
    /**
     * Handle mouse events
     * @param event
     */
    void handleMouseKeys(sf::Event* event);
    
    /**
     * Handle buttons events (click, hover)
     * 
     * @param event
     */
    void handleUiButtonsEvents(sf::Event* event);
};

#endif /* EVENT_H */

