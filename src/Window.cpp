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
    sprite = new sf::Sprite();
   /// sprite->setTextureRect(sf::IntRect(0, 100, 50, 50));
    sprite->setPosition(0.f, -300.f);
    texture = new sf::Texture();
    if (!texture->loadFromFile("resources/images/background.png")) {
        std::cout << "Image images/background.png is not found" << std::endl;
        throw;
    }
    sprite->setTexture(*texture);
}

void Window::drawAll(GameState* gameState)
{
    if (gameState->isGameStarted) {
        sf::Vector2i target = mapCoordsToPixel(sf::Vector2f(
            (float)gameState->objects->player->getX(),
            (float)gameState->objects->player->getY())
        );

        sf::Vector2f backgroundCoords = mapPixelToCoords(sf::Vector2i(0, 0));
        sprite->setPosition(backgroundCoords.x, backgroundCoords.y - 300);

        if (target.x >= 800) {
            sprite->move(500.f, 0.f);
            gameState->view->move(600.f, 0.f);
        } else if (target.x <= 50) {
            sprite->move(-800.f, 0.f);
            gameState->view->move(-600.f, 0.f);  
        }

        setView(*gameState->view);
    }
    

    sf::Time elapsed = gameState->clock.getElapsedTime();
    float dt = elapsed.asMilliseconds() / 1000.f;

    clear(sf::Color::White);

    draw(*sprite);
    // TODO: this is a bad realization. Need to refactor and simplify
    for (unsigned i = 0; i < gameState->objects->buttons.size(); i++) {
        gameState->objects->buttons[i]->draw(this, dt);
    }

    for (unsigned i = 0; i < gameState->objects->background.size(); i++) {
        gameState->objects->background[i]->draw(this, dt);
    }
    for (unsigned i = 0; i < gameState->objects->crystals.size(); i++) {
        gameState->objects->crystals[i]->draw(this, dt);
    }

    for (unsigned i = 0; i < gameState->objects->bullets.size(); i++) {
        gameState->objects->bullets[i]->draw(this, dt);
    }
    
    for (unsigned i = 0; i < gameState->objects->playable.size(); i++) {
        gameState->objects->playable[i]->draw(this, dt);
        if (((Shooter*)gameState->objects->playable[i])->isNeedToDie) {
            gameState->objects->playable.erase(gameState->objects->playable.begin() + i);
        }
        
    }

    gameState->clock.restart();
    display();
}
