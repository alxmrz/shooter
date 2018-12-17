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
            app->close();
        } else if (event.type == sf::Event::KeyPressed) {
            handleMainKeys(event);
        } else if (event.type == sf::Event::KeyReleased) {
            handleArrowKeysReleased(event);
        } else  if (event.type == sf::Event::MouseWheelScrolled) { 
            handleMouseEvents(event);
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                handleUiButtonsClick(event);
            }
        } else if (event.type == sf::Event::MouseMoved) {
            handleUiButtonsHover(event);
        }
    }
    if (gameState->isGameStarted) {
        handleRealTimeKeyboardState();
    }   
}

void Event::handleMainKeys(sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Escape) {
        app->close();
    } 
}

void Event::handleArrowKeysReleased(sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Up) {
            player->stopJumping();
    } else if (event.key.code == sf::Keyboard::Left) {
         player->stopMoving();
    } else if (event.key.code == sf::Keyboard::Right) {
         player->stopMoving();
    }
}

void Event::handleMouseEvents(sf::Event& event)
{
    if (gameState->isGameStarted) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            if (event.mouseWheelScroll.delta == -1) {
                gameState->view->zoom(1.5);
            } else {
                gameState->view->zoom(0.5);
            }
        } else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
            gameState->view->zoom(-1.f);
        } else {
            std::cout << "wheel type: unknown" << std::endl;
        }
    }
}

void Event::handleUiButtonsClick(sf::Event& event)
{
    for (unsigned i = 0; i < gameState->objects->buttons.size(); i++) {
        Button* button = static_cast<Button*>(gameState->objects->buttons[i]);
        
        bool isHovered = button->collidePoint(event.mouseButton.x, event.mouseButton.y);
        if (isHovered) {
            button->clicked();
            
            std::string id = button->getId();
            if (id == "start") {
                gameState->startNewGame();
                this->player = gameState->objects->player;
                break;
            } else if (id == "exit") {
                app->close();
            }
        } 
    }
}

void Event::handleUiButtonsHover(sf::Event& event)
{
    for (unsigned i = 0; i < gameState->objects->buttons.size(); i++) {
        Button* button = static_cast<Button*>(gameState->objects->buttons[i]);

        bool isHovered = button->collidePoint(event.mouseMove.x, event.mouseMove.y);
        if (isHovered) {
            button->hovered();
        } else {
            button->defaultState();
        }
    }
}

void Event::handleRealTimeKeyboardState()
{
    handleArrowKeys();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        player->fire();
    } 
}

void Event::handleArrowKeys()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {       
        player->jump();
    } 
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player->move("left");
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { 
        player->move("right");
    } 
}