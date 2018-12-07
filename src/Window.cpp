#include "Window.h"
#include "GameState.h"
#include <SFML/Graphics.hpp>

Window::Window(sf::VideoMode mode, 
            const sf::String& title, 
            sf::Uint32 style, 
            const sf::ContextSettings& settings)
        : sf::RenderWindow(mode, title, style, settings)
{
}

void Window::drawAll(GameState* gameState)
{
    setView(*gameState->view);
    
    clear(sf::Color::White);
    
    for (unsigned i = 0; i < gameState->gameObjects.size(); i++) {
        //sf::RenderTarget::draw(*gameState->gameObjects[i]->getDrawForm());
        gameState->gameObjects[i]->draw(this);
    }
    
    display();
}
