#ifndef EVENT_H
#define EVENT_H

#include <SFML/Graphics.hpp>

class Application;
class GameState;
class Unit;

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
    void handleArrowKeysReleased(sf::Event& event);
    
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

#endif /* EVENT_H */

