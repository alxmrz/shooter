#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SFML/Graphics.hpp>

#include "Event.h"

class Application;
class GameState;
class Unit;

class EventHandler {
public:
    EventHandler();
    EventHandler(Application* app, GameState* gameState);
    virtual ~EventHandler();
    
    /**
     * Handle all events
     */
    Event handle();
private:
    Application* app;
    GameState* gameState;
    Unit* player;

    /**
     * Handle events of usual keys
     * 
     * @param event
     */
    void handleMainKeys(sf::Event& event);
    
    /**
     * Handle arrow keys events
     * 
     * @param event
     */
    void handleArrowKeys();

    /**
     * Handle mouse events
     * @param event
     */
    void handleMouseEvents(sf::Event& event);
    
    /**
     * Handle buttons events (click, hover)
     * 
     * @param event
     */
    void handleUiButtonsClick(sf::Event& event);
    void handleUiButtonsHover(sf::Event& event);
    void handleRealTimeKeyboardState();
};

#endif /* EVENT_HANDLER_H */

