#ifndef WINDOW_H
#define WINDOW_H

#include "GameState.h"
#include <SFML/Graphics.hpp>

class Window: public sf::RenderWindow
{
public:
    Window(sf::VideoMode mode, 
            const sf::String& title, 
            sf::Uint32 style = sf::Style::Default, 
            const sf::ContextSettings& settings = sf::ContextSettings());
    void draw(GameState*);
private:

};

#endif /* WINDOW_H */

