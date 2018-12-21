#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "GameState.h"
#include "CObject.h"
#include "objects/Shooter.h"

Window::Window(sf::VideoMode mode,
        const sf::String& title,
        sf::Uint32 style,
        const sf::ContextSettings& settings)
: sf::RenderWindow(mode, title, style, settings)
{
}

void Window::draw(GameState* gameState)
{
    if (gameState->isGameStarted) {       
        updateView(gameState);
    }
    
    clear(sf::Color::White);
    gameState->objects->draw(this, gameState->getElapsedTime());
    
    /**
     * TODO: Need to fix the bug with the time animation. 
     * If the line is replaced lower objects animation will be much slower
     * Key game process must not depend on this
     * The same problem with gameState::resetElapsedTime() call in gameState::update()
     */
    gameState->clock.restart();
    
    display();
    
    
}

void Window::updateView(GameState* gameState)
{
    sf::Vector2i playerWindowCoords = mapCoordsToPixel(sf::Vector2f(
            (float) gameState->objects->player->getX(),
            (float) gameState->objects->player->getY())
            );
    /*if (playerWindowCoords.x >= 800) {
        gameState->view->move(600.f, 0.f);
    } else if (playerWindowCoords.x <= 50) {
        gameState->view->move(-600.f, 0.f);
    }*/
    gameState->view->setCenter(
            gameState->objects->player->getX(),
            gameState->objects->player->getY()
    );
    setView(*gameState->view);
}