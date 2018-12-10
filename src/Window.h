#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>

class GameState;

class Window : public sf::RenderWindow {
public:
    using sf::RenderTarget::draw;

    Window(sf::VideoMode mode,
            const sf::String& title,
            sf::Uint32 style = sf::Style::Default,
            const sf::ContextSettings& settings = sf::ContextSettings());

    void drawAll(GameState*);
private:

};

#endif /* WINDOW_H */

