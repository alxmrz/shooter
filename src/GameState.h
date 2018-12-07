#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Application.h"
#include "CObject.h"
#include "Builder.h"

class Application;
class CObject;
class Builder;

class GameState 
{
public:
    sf::CircleShape shape;
    sf::Clock clock;
    Builder* builder;
    Builder* player;
    sf::View* view;
    
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