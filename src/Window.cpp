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

void Window::drawAll(GameState* gameState)
{
    // set game camera. Need to update every frame
    setView(*gameState->view);

    sf::Time elapsed = gameState->clock.getElapsedTime();
    float dt = elapsed.asMilliseconds() / 1000.f;

    clear(sf::Color::White);


    // TODO: this is a bad realization. Need to refactor and simplify
    for (unsigned i = 0; i < gameState->objects.buttons.size(); i++) {
        gameState->objects.buttons[i]->draw(this, dt);
    }

    for (unsigned i = 0; i < gameState->objects.playable.size(); i++) {
        gameState->objects.playable[i]->draw(this, dt);
        if (((Shooter*)gameState->objects.playable[i])->isNeedToDie) {
            gameState->objects.playable.erase(gameState->objects.playable.begin() + i);
        }
        
    }

    for (unsigned i = 0; i < gameState->objects.background.size(); i++) {
        gameState->objects.background[i]->draw(this, dt);
    }

    for (unsigned i = 0; i < gameState->objects.bullets.size(); i++) {
        gameState->objects.bullets[i]->draw(this, dt);
    }

    gameState->clock.restart();
    display();
}
