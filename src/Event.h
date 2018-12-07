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
    void handle();
private:
    Application* app;
    GameState* gameState;
    
    void handleKeys(sf::Event* event);
    void handelArrowKeys(sf::Event* event);
    void handleMouseKeys(sf::Event* event);
    void handleUiButtonsEvents(sf::Event* event);
};

#endif /* EVENT_H */

