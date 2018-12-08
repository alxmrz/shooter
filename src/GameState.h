#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Application.h"
#include "CObject.h"
#include "Builder.h"
#include "ui/Button.h"

class Application;
class CObject;
class Builder;
class Shooter;

struct GameObjects 
{
    GameObjects();
    
    std::vector<CObject*> buttons;
    std::vector<CObject*> background;
    std::vector<Builder*> playable;
    Shooter* player;
    
    virtual ~GameObjects();
    std::vector<CObject*>* all();
    void reset();
};

class GameState 
{
public:
    sf::CircleShape shape;
    sf::Clock clock;
    Builder* builder;
    Builder* player;
    sf::View* view;
    GameObjects objects;
    
    std::vector<sf::Text*> texts;
    
    int frame = 0;
    bool right = true;
    bool left = false;

    GameState();
    GameState(Application* app);
    virtual ~GameState();
    void update();
    
    std::vector<CObject*> gameObjects;
private:
    Application* app;
    
};

#endif