#include "Window.h"
#include <SFML/Graphics.hpp>

Window::Window(sf::VideoMode mode, 
            const sf::String& title, 
            sf::Uint32 style, 
            const sf::ContextSettings& settings)
        : sf::RenderWindow(mode, title, style, settings)
{
}

void Window::draw(GameState* gameState)
{
    setView(*gameState->view);
    
    clear(sf::Color::White);
    for (unsigned i = 0; i < gameState->texts.size(); i++) {
        sf::RenderTarget::draw(*gameState->texts[i]);
    }

    sf::RenderTarget::draw(gameState->shape);
    sf::RenderTarget::draw(*gameState->builder->getDrawForm());
    sf::RenderTarget::draw(*gameState->builder1->getDrawForm());
    
    display();
}
