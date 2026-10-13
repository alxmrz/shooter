#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "GameObjects.h"
#include "Application.h"
#include "CObject.h"
#include "Scene.h"
#include "objects/units/Unit.h"
#include "objects/interactive/Bullet.h"
#include "objects/interactive/Ammunition.h"
#include "Fabric.h"
#include "AI.h"
#include <typeinfo>

#include "Event.h"

GameState::GameState()
{
}

GameState::GameState(Application* app)
{
    this->app = app;
    objects = new GameObjects(this);
    backgroundMusic = objects->fabric->getBackgroundSound();
}

GameState::~GameState()
{
    objects->clear();
    delete backgroundMusic;
    delete objects;
}

void GameState::update(Event event)
{
    if (!isGameplayRunning()) {
        return;
    }

    updateRunningGame(event);
}

bool GameState::isGameplayRunning()
{
    return isGameStarted && !isGamePaused;
}

void GameState::updateRunningGame(Event event)
{
    // Пока диалог открыт, игровой мир заморожен, но диалог продолжает печатать
    if (objects->isDialogueOpen()) {
        updateDialogue(event);
        return;
    }

    updatePlayerDeath();
    updateGameplay(event);
}

void GameState::updateDialogue(Event event)
{
    objects->update(event, getElapsedTime());
    resetElapsedTime();
}

void GameState::updateGameplay(Event event)
{
    if (objects->gameOver) {
        return;
    }

    updateWorld(event);
}

void GameState::updateWorld(Event event)
{
    objects->update(event, getElapsedTime());
    resetElapsedTime();
}

void GameState::updatePlayerDeath()
{
    if (!isDeathAnimationFinished()) {
        return;
    }

    objects->showGameOverScreen();
}

bool GameState::isDeathAnimationFinished()
{
    return isPlayerDeathPending() && objects->player->isMustBeDeleted();
}

bool GameState::isPlayerDeathPending()
{
    return hasDeadPlayer() && !objects->gameOver;
}

bool GameState::hasDeadPlayer()
{
    return objects->player != nullptr && objects->player->isDead();
}

void GameState::resetElapsedTime()
{
    sf::Time elapsed = this->clock.getElapsedTime();
    elapsedTime = elapsed.asMilliseconds() / 1000.f;
}

void GameState::playBackgroundMusic()
{
    backgroundMusic->setLoop(true);
    backgroundMusic->play();
}

void GameState::stopBackgroundMusic()
{
    backgroundMusic->setLoop(false);
    backgroundMusic->stop();
}

void GameState::startNewGame()
{
    isGameStarted = true;
    playBackgroundMusic();
    app->scene->initNewGame();
}

float GameState::getElapsedTime()
{
    return elapsedTime;
}

