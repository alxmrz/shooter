#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>

class GameState;

class Window : public sf::RenderWindow {
public:
    using sf::RenderTarget::draw;

    Window();
    Window(sf::VideoMode mode,
            const sf::String& title,
            sf::Uint32 style = sf::Style::Default,
            const sf::ContextSettings& settings = sf::ContextSettings());

    /**
     * Draw all objects of current scene
     * 
     * @param 
     */
    void draw(GameState*);
    void init();
    void changeFullScreenMode();
private:
    bool isFullScreen = false;
    /**
     * @var fps of the app we are needed
     */
    int fps = 120;
    void updateView(GameState* gameState);
};

#endif /* WINDOW_H */

