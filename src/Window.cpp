#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "GameState.h"
#include "CObject.h"
#include "objects/Unit.h"

Window::Window(sf::VideoMode mode,
        const sf::String& title,
        sf::Uint32 style,
        const sf::ContextSettings& settings)
: sf::RenderWindow(mode, title, style, settings)
{
}

Window::Window(): sf::RenderWindow()
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
    /*sf::Vector2i playerWindowCoords = mapCoordsToPixel(sf::Vector2f(
            gameState->objects->player->getX(),
            gameState->objects->player->getY())
            );*/
    gameState->view->setCenter(
            gameState->objects->player->getX(),
            gameState->objects->player->getY()-200
    );
    setView(*gameState->view);
}

void Window::init()
{
    create(sf::VideoMode(900, 600), "Shooter");
    setFramerateLimit(fps);
}

void Window::changeFullScreenMode()
{
    close();
    if (isFullScreen) {
        isFullScreen = false;
        create(sf::VideoMode(900, 600), "Shooter");
    } else {
        isFullScreen = true;
        create(sf::VideoMode::getDesktopMode(), "Shooter", sf::Style::Fullscreen);
    }      
    setFramerateLimit(fps);
}