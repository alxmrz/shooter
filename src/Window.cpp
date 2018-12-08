#include <vector>
#include "Window.h"
#include "GameState.h"
#include "CObject.h"
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
  
    
    
    for (unsigned i = 0; i < gameState->objects.buttons.size(); i++) {
        gameState->objects.buttons[i]->draw(this);
    }
    
    for (unsigned i = 0; i < gameState->objects.playable.size(); i++) {
        gameState->objects.playable[i]->draw(this);
    }
    
    
    display();
}
