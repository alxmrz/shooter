#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Builder.h"

class Application
{
public:
    Application();
    ~Application();
    int run();
private:
    sf::RenderWindow* window;
    sf::Event event;
    sf::CircleShape shape;
    sf::Clock clock;
    Builder* builder;
    std::vector<sf::Text*> texts;
    int frame = 0;
    bool right = true;
    bool left = false;
    
    sf::Text* makeText(std::string str, int x, int );
    void initScene();
    void handleEvents();
    void updateGame();
    void drawGameObjects();
    
};


#endif /* APPLICATION_H */

