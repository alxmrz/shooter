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

    /**
     * Draw all objects of current scene
     * 
     * @param 
     */
    void drawAll(GameState*);
private:
    /**
     * @var Sprite for drawing
     */
    sf::Sprite* sprite;
    
    /**
     * Texture for sprite (Texture loads image). Sprite contains the texture
     */
    sf::Texture* texture;
};

#endif /* WINDOW_H */

