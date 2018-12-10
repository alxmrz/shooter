#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Event.h"
#include "Window.h"
#include "objects/Shooter.h"
#include "Scene.h"
#include "Application.h"
#include "GameState.h"
#include "ui/Button.h"

Event::Event()
{
}

Event::Event(Application* app, GameState* gameState)
{
    this->app = app;
    this->gameState = gameState;
}

Event::~Event()
{
}

void Event::handle()
{
    sf::Event event;

    while (app->window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            app->window->close();
        }

        handleKeys(&event);
        handelArrowKeys(&event);
        handleMouseKeys(&event);
        handleUiButtonsEvents(&event);

    }
}

void Event::handleKeys(sf::Event* event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        gameState->objects.player->fire();
    }
}

void Event::handelArrowKeys(sf::Event* event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        gameState->objects.player->move(0, -50);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {

    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        gameState->objects.player->move(-10, 0);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        gameState->objects.player->move(10, 0);
    }
}

void Event::handleMouseKeys(sf::Event* event)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

    } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

    }

    if (event->type == sf::Event::MouseWheelScrolled) {
        if (event->mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            if (event->mouseWheelScroll.delta == -1) {
                gameState->view->zoom(1.5);
            } else {
                gameState->view->zoom(0.5);
            } else if (event->mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
            gameState->view->zoom(-1.f);
        else {
            std::cout << "wheel type: unknown" << std::endl;
        }
    }
}

void Event::handleUiButtonsEvents(sf::Event* event)
{
    for (unsigned i = 0; i < gameState->objects.buttons.size(); i++) {
        Button* button = (Button*) gameState->objects.buttons[i];
        sf::Vector2i globalPosition = sf::Mouse::getPosition(*app->window);
        
        bool isHovered = button->collidePoint(globalPosition.x, globalPosition.y);
        bool isClicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (isHovered && isClicked) {
            button->clicked();
            std::string id = button->getId();

            if (id == "start") {
                gameState->isGameStarted = true;
                app->scene->initNewGame();
                break;
            } else if (id == "exit") {
                app->window->close();
            }
        } else if (isHovered) {
            button->hovered();
        } else {
            button->defaultState();
        }
    }
}