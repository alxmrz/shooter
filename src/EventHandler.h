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
    bool handleUiButtonsClick(sf::Event& event);
    void handleUiButtonsHover(sf::Event& event);
    void handleRealTimeKeyboardState();

    /**
     * Handle a left mouse button press: UI buttons first, then a shot.
     */
    void handleLeftClick(sf::Event& event);

    /**
     * Fire the player weapon if a game is running and the player is alive.
     */
    void handlePlayerFire();

    /**
     * Update the crosshair and the player aim target from the mouse position.
     */
    void updateAimTarget();
    sf::Vector2f currentMouseWorld();

    /**
     * Is there an active game with a living player?
     */
    bool isGameplayActive();

    void handleSpaceKey();
};

#endif /* EVENT_HANDLER_H */

