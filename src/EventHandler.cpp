#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Event.h"
#include "EventHandler.h"
#include "Window.h"
#include "objects/units/Unit.h"
#include "Scene.h"
#include "Application.h"
#include "GameState.h"
#include "GameObjects.h"
#include "objects/interactive/Aim.h"
#include "ui/Button.h"

EventHandler::EventHandler()
{
}

EventHandler::EventHandler(Application* app, GameState* gameState)
{
    this->app = app;
    this->gameState = gameState;
}

EventHandler::~EventHandler()
{
}

Event EventHandler::handle()
{
    Event result;
    sf::Event event;

    updateAimTarget();

    while (app->window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            app->close();
            result.SetType(Event::Type::Closed, event);
        } else if (event.type == sf::Event::KeyPressed) {
            handleMainKeys(event);
            result.SetType(Event::Type::KeyPressed, event);
        } else if (event.type == sf::Event::KeyReleased) {
            result.SetType(Event::Type::KeyReleased, event);
        } else  if (event.type == sf::Event::MouseWheelScrolled) { 
            handleMouseEvents(event);
            result.SetType(Event::Type::MouseWheelScrolled, event);
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                handleLeftClick(event);
            }
            result.SetType(Event::Type::MouseButtonPressed, event);
        } else if (event.type == sf::Event::MouseMoved) {
            handleUiButtonsHover(event);
            result.SetType(Event::Type::MouseMoved, event);
        }
    }
    if (gameState->isGameStarted) {
        handleRealTimeKeyboardState();
    }

    return result;
}

void EventHandler::handleMainKeys(sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Escape) {
        app->close();
    } else if (event.key.code == sf::Keyboard::F) {
        app->window->changeFullScreenMode();
    }
}

void EventHandler::handleMouseEvents(sf::Event& event)
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

bool EventHandler::handleUiButtonsClick(sf::Event& event)
{
    // Преобразуем координаты мыши в мировые координаты (учитывая view)
    sf::Vector2f mousePos = app->window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    
    for (unsigned i = 0; i < gameState->objects->buttons.size(); i++) {
        Button* button = static_cast<Button*>(gameState->objects->buttons[i]);
        
        bool isHovered = button->collidePoint(mousePos.x, mousePos.y);
        if (isHovered) {
            button->clicked();
            
            std::string id = button->getId();
            if (id == "start") {
                gameState->startNewGame();
                this->player = gameState->objects->player;
            } else if (id == "exit") {
                app->close();
            } else if (id == "restart") {
                gameState->objects->restartGame();
                this->player = gameState->objects->player;
            }
            return true;
        }
    }

    return false;
}

void EventHandler::handleLeftClick(sf::Event& event)
{
    if (handleUiButtonsClick(event)) {
        return;
    }

    handlePlayerFire();
}

void EventHandler::handlePlayerFire()
{
    if (!isGameplayActive()) {
        return;
    }

    player->attack();
}

void EventHandler::updateAimTarget()
{
    if (!isGameplayActive() || gameState->objects->aim == nullptr) {
        return;
    }

    sf::Vector2f mouseWorld = currentMouseWorld();
    gameState->objects->aim->setTarget(mouseWorld.x, mouseWorld.y);
    player->setAimTarget(mouseWorld.x, mouseWorld.y);
}

sf::Vector2f EventHandler::currentMouseWorld()
{
    sf::Vector2i pixel = sf::Mouse::getPosition(*app->window);
    return app->window->mapPixelToCoords(pixel);
}

bool EventHandler::isGameplayActive()
{
    return gameState->isGameStarted
        && !gameState->objects->gameOver
        && player != nullptr
        && !player->isDead();
}

void EventHandler::handleUiButtonsHover(sf::Event& event)
{
    // Преобразуем координаты мыши в мировые координаты (учитывая view)
    sf::Vector2f mousePos = app->window->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
    
    for (unsigned i = 0; i < gameState->objects->buttons.size(); i++) {
        Button* button = static_cast<Button*>(gameState->objects->buttons[i]);

        bool isHovered = button->collidePoint(mousePos.x, mousePos.y);
        if (isHovered) {
            button->hovered();
        } else {
            button->defaultState();
        }
    }
}

void EventHandler::handleRealTimeKeyboardState()
{
    // Если игра окончена (gameOver) или игрок умер, не обрабатываем ввод
    if (!isGameplayActive()) {
        return;
    }
    
    handleArrowKeys();
    handleSpaceKey();
}

void EventHandler::handleSpaceKey()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        player->attack();
    }
}

void EventHandler::handleArrowKeys()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        if (player->canJump()) {
            player->jump();
        }
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player->move("left");
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player->move("right");
    }
}