#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
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
        dispatchEvent(event, result);
    }

    handleRealTimeKeyboardState();

    return result;
}

void EventHandler::dispatchEvent(sf::Event& event, Event& result)
{
    const EventDispatchTable& table = eventDispatchTable();
    EventDispatchTable::const_iterator it = table.find(event.type);

    if (it == table.end()) {
        return;
    }

    (this->*(it->second))(event, result);
}

const EventHandler::EventDispatchTable& EventHandler::eventDispatchTable()
{
    static const EventDispatchTable table = {
        {sf::Event::Closed, &EventHandler::handleClosedEvent},
        {sf::Event::KeyPressed, &EventHandler::handleKeyPressedEvent},
        {sf::Event::KeyReleased, &EventHandler::handleKeyReleasedEvent},
        {sf::Event::MouseWheelScrolled, &EventHandler::handleMouseScrolledEvent},
        {sf::Event::MouseButtonPressed, &EventHandler::handleMouseButtonPressedEvent},
        {sf::Event::MouseMoved, &EventHandler::handleMouseMovedEvent}
    };
    return table;
}

void EventHandler::handleClosedEvent(sf::Event& event, Event& result)
{
    app->close();
    result.SetType(Event::Type::Closed, event);
}

void EventHandler::handleKeyPressedEvent(sf::Event& event, Event& result)
{
    handleMainKeys(event);
    handleDialogueKey(event);
    result.SetType(Event::Type::KeyPressed, event);
}

void EventHandler::handleKeyReleasedEvent(sf::Event& event, Event& result)
{
    result.SetType(Event::Type::KeyReleased, event);
}

void EventHandler::handleMouseScrolledEvent(sf::Event& event, Event& result)
{
    handleMouseEvents(event);
    result.SetType(Event::Type::MouseWheelScrolled, event);
}

void EventHandler::handleMouseButtonPressedEvent(sf::Event& event, Event& result)
{
    if (isLeftButton(event)) {
        handleLeftClick(event);
    }
    result.SetType(Event::Type::MouseButtonPressed, event);
}

void EventHandler::handleMouseMovedEvent(sf::Event& event, Event& result)
{
    handleUiButtonsHover(event);
    result.SetType(Event::Type::MouseMoved, event);
}

bool EventHandler::isLeftButton(sf::Event& event)
{
    return event.mouseButton.button == sf::Mouse::Left;
}

void EventHandler::handleMainKeys(sf::Event& event)
{
    if (isEscapeKey(event)) {
        app->close();
        return;
    }

    toggleFullScreenIfRequested(event);
}

bool EventHandler::isEscapeKey(sf::Event& event)
{
    return event.key.code == sf::Keyboard::Escape;
}

bool EventHandler::isFullScreenKey(sf::Event& event)
{
    return event.key.code == sf::Keyboard::F;
}

void EventHandler::toggleFullScreenIfRequested(sf::Event& event)
{
    if (isFullScreenKey(event)) {
        app->window->changeFullScreenMode();
    }
}

void EventHandler::handleArrowKeys()
{
    handleJumpKey();
    handleHorizontalKeys();
}

void EventHandler::handleJumpKey()
{
    if (!isUpPressed()) {
        return;
    }

    jumpIfPossible();
}

bool EventHandler::isUpPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
}

void EventHandler::jumpIfPossible()
{
    if (player->canJump()) {
        player->jump();
    }
}

void EventHandler::handleHorizontalKeys()
{
    if (isLeftPressed()) {
        player->move("left");
        return;
    }

    moveRightIfPressed();
}

bool EventHandler::isLeftPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
}

bool EventHandler::isRightPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

void EventHandler::moveRightIfPressed()
{
    if (isRightPressed()) {
        player->move("right");
    }
}

void EventHandler::handleMouseEvents(sf::Event& event)
{
    if (!gameState->isGameStarted) {
        return;
    }

    applyWheelZoom(event.mouseWheelScroll);
}

void EventHandler::applyWheelZoom(const sf::Event::MouseWheelScrollEvent& scroll)
{
    if (scroll.wheel == sf::Mouse::VerticalWheel) {
        applyVerticalZoom(scroll.delta);
        return;
    }

    applyHorizontalZoom(scroll.wheel);
}

void EventHandler::applyVerticalZoom(float delta)
{
    if (delta == -1) {
        gameState->view->zoom(1.5);
        return;
    }

    gameState->view->zoom(0.5);
}

void EventHandler::applyHorizontalZoom(sf::Mouse::Wheel wheel)
{
    if (wheel == sf::Mouse::HorizontalWheel) {
        gameState->view->zoom(-1.f);
        return;
    }

    std::cout << "wheel type: unknown" << std::endl;
}

bool EventHandler::handleUiButtonsClick(sf::Event& event)
{
    sf::Vector2f mousePos = worldMouseFromButton(event);
    std::vector<CObject*>& buttons = gameState->objects->buttons;

    return std::any_of(buttons.begin(), buttons.end(), ButtonClicker{this, mousePos});
}

bool EventHandler::ButtonClicker::operator()(CObject* object) const
{
    return owner->clickButton(static_cast<Button*>(object), mousePos);
}

bool EventHandler::clickButton(Button* button, sf::Vector2f mousePos)
{
    if (!button->collidePoint(mousePos.x, mousePos.y)) {
        return false;
    }

    activateButton(button);
    return true;
}

void EventHandler::activateButton(Button* button)
{
    button->clicked();
    performButtonAction(button->getId());
}

void EventHandler::performButtonAction(const std::string& id)
{
    const ButtonActionTable& actions = buttonActions();
    ButtonActionTable::const_iterator it = actions.find(id);

    if (it == actions.end()) {
        return;
    }

    (this->*(it->second))();
}

const EventHandler::ButtonActionTable& EventHandler::buttonActions()
{
    static const ButtonActionTable table = {
        {"start", &EventHandler::startGameFromButton},
        {"exit", &EventHandler::exitFromButton}
    };
    return table;
}

void EventHandler::startGameFromButton()
{
    gameState->startNewGame();
    this->player = gameState->objects->player;
}

void EventHandler::exitFromButton()
{
    app->close();
}

void EventHandler::handleUiButtonsHover(sf::Event& event)
{
    sf::Vector2f mousePos = worldMouseFromMove(event);

    for (unsigned i = 0; i < gameState->objects->buttons.size(); i++) {
        updateButtonHover(static_cast<Button*>(gameState->objects->buttons[i]), mousePos);
    }
}

void EventHandler::updateButtonHover(Button* button, sf::Vector2f mousePos)
{
    if (button->collidePoint(mousePos.x, mousePos.y)) {
        button->hovered();
        return;
    }

    button->defaultState();
}

void EventHandler::handleRealTimeKeyboardState()
{
    // The player may not be controlled while a game is not running, while a
    // dialogue is open, or on the same frame a dialogue key was consumed.
    if (!shouldHandleRealTimeKeyboard()) {
        return;
    }

    handleArrowKeys();
}

void EventHandler::handleLeftClick(sf::Event& event)
{
    if (handleDeathChoice(event) || handleUiButtonsClick(event)) {
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

sf::Vector2f EventHandler::worldMouseFromButton(sf::Event& event)
{
    return app->window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
}

sf::Vector2f EventHandler::worldMouseFromMove(sf::Event& event)
{
    return app->window->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
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
    return isGameplayActive() && !dialogueKeyConsumed;
}
