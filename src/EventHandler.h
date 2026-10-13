#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "Event.h"

class Application;
class GameState;
class Unit;
class Button;
class CObject;

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
    typedef void (EventHandler::*EventMethod)(sf::Event&, Event&);
    typedef std::map<sf::Event::EventType, EventMethod> EventDispatchTable;
    typedef void (EventHandler::*ButtonMethod)();
    typedef std::map<std::string, ButtonMethod> ButtonActionTable;

    Application* app;
    GameState* gameState;
    Unit* player;

    /**
     * Route a single SFML event to its handler through the dispatch table.
     */
    void dispatchEvent(sf::Event& event, Event& result);
    const EventDispatchTable& eventDispatchTable();
    void handleClosedEvent(sf::Event& event, Event& result);
    void handleKeyPressedEvent(sf::Event& event, Event& result);
    void handleKeyReleasedEvent(sf::Event& event, Event& result);
    void handleMouseScrolledEvent(sf::Event& event, Event& result);
    void handleMouseButtonPressedEvent(sf::Event& event, Event& result);
    void handleMouseMovedEvent(sf::Event& event, Event& result);
    bool isLeftButton(sf::Event& event);

    /**
     * Handle events of usual keys
     * 
     * @param event
     */
    void handleMainKeys(sf::Event& event);
    bool isEscapeKey(sf::Event& event);
    bool isFullScreenKey(sf::Event& event);
    void toggleFullScreenIfRequested(sf::Event& event);
    
    /**
     * Handle arrow keys events
     * 
     * @param event
     */
    void handleArrowKeys();
    void handleJumpKey();
    bool isUpPressed();
    void jumpIfPossible();
    void handleHorizontalKeys();
    bool isLeftPressed();
    bool isRightPressed();
    void moveRightIfPressed();

    /**
     * Handle mouse events
     * @param event
     */
    void handleMouseEvents(sf::Event& event);
    void applyWheelZoom(const sf::Event::MouseWheelScrollEvent& scroll);
    void applyVerticalZoom(float delta);
    void applyHorizontalZoom(sf::Mouse::Wheel wheel);
    
    /**
     * Handle buttons events (click, hover)
     * 
     * @param event
     */
    bool handleUiButtonsClick(sf::Event& event);
    void handleUiButtonsHover(sf::Event& event);
    bool clickButton(Button* button, sf::Vector2f mousePos);
    void activateButton(Button* button);
    void performButtonAction(const std::string& id);
    const ButtonActionTable& buttonActions();
    void startGameFromButton();
    void exitFromButton();
    void updateButtonHover(Button* button, sf::Vector2f mousePos);
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
    sf::Vector2f worldMouseFromButton(sf::Event& event);
    sf::Vector2f worldMouseFromMove(sf::Event& event);

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

    struct ButtonClicker {
        EventHandler* owner;
        sf::Vector2f mousePos;

        bool operator()(CObject* object) const;
    };
};

#endif /* EVENT_HANDLER_H */
