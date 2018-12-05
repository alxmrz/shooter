#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "CObject.h"
#include "Builder.h"
#include <vector>

class Application;

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
    sf::Text* makeText(std::string str, int x, int y);
    std::vector<CObject*> gameObjects;
private:
    Application* app;
    
};


