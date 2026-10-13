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
     * Handle a left mouse button press: death dialog choice, then UI buttons,
     * then a shot.
     */
    void handleLeftClick(sf::Event& event);

    /**
     * Handle a click on the death dialog's choice buttons.
     */
    bool handleDeathChoice(sf::Event& event);
    bool clickDeathChoice(sf::Event& event);
    void applyDeathChoice(int choice);
    bool isDeathDialogueOpen();

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
     * Handle a Space press as dialogue advance when the dialogue is open.
     */
    void handleDialogueKey(sf::Event& event);
    bool isDialogueSpace(sf::Event& event);
    bool isSpaceKey(sf::Event& event);
    bool isDialogueOpen();

    /**
     * Is there an active game with a living player and no open dialogue?
     */
    bool isGameplayActive();
    bool isGameRunning();
    bool isMatchLive();
    bool hasLivingPlayer();
    bool shouldHandleRealTimeKeyboard();

    bool dialogueKeyConsumed = false;
};

#endif /* EVENT_HANDLER_H */

