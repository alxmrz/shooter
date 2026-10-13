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

    dialogueKeyConsumed = false;

    // The player may have been recreated by a stage restart, so track the
    // current instance instead of a stale pointer.
    player = gameState->objects->player;

    updateAimTarget();

    while (app->window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            app->close();
            result.SetType(Event::Type::Closed, event);
        } else if (event.type == sf::Event::KeyPressed) {
            handleMainKeys(event);
            handleDialogueKey(event);
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
    if (shouldHandleRealTimeKeyboard()) {
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
            }
            return true;
        }
    }

    return false;
}

void EventHandler::handleLeftClick(sf::Event& event)
{
    if (handleDeathChoice(event)) {
        return;
    }

    if (handleUiButtonsClick(event)) {
        return;
    }

    handlePlayerFire();
}

bool EventHandler::handleDeathChoice(sf::Event& event)
{
    if (!isDeathDialogueOpen()) {
        return false;
    }

    return clickDeathChoice(event);
}

bool EventHandler::clickDeathChoice(sf::Event& event)
{
    int choice = gameState->objects->deathChoiceAt(event.mouseButton.x, event.mouseButton.y);
    if (choice < 0) {
        return false;
    }

    applyDeathChoice(choice);
    return true;
}

void EventHandler::applyDeathChoice(int choice)
{
    if (choice != 0) {
        app->close();
        return;
    }

    gameState->objects->restartGame();
    this->player = gameState->objects->player;
}

bool EventHandler::isDeathDialogueOpen()
{
    return gameState->objects->isDeathDialogueOpen();
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
    return isGameRunning() && !isDialogueOpen();
}

bool EventHandler::isGameRunning()
{
    return isMatchLive() && hasLivingPlayer();
}

bool EventHandler::isMatchLive()
{
    return gameState->isGameStarted && !gameState->objects->gameOver;
}

bool EventHandler::hasLivingPlayer()
{
    return player != nullptr && !player->isDead();
}

bool EventHandler::shouldHandleRealTimeKeyboard()
{
    return gameState->isGameStarted && !dialogueKeyConsumed;
}

void EventHandler::handleDialogueKey(sf::Event& event)
{
    if (!isDialogueSpace(event)) {
        return;
    }

    gameState->objects->advanceDialogue();
    dialogueKeyConsumed = true;
}

bool EventHandler::isDialogueSpace(sf::Event& event)
{
    return isSpaceKey(event) && isDialogueOpen() && !isDeathDialogueOpen();
}

bool EventHandler::isSpaceKey(sf::Event& event)
{
    return event.key.code == sf::Keyboard::Space;
}

bool EventHandler::isDialogueOpen()
{
    return gameState->objects->isDialogueOpen();
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