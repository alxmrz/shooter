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
    if (isGameStarted && !isGamePaused) {
        // Проверяем, умер ли игрок
        if (objects->player && objects->player->isDead() && !objects->gameOver) {
            // Ждем завершения анимации смерти (mustBeDeleted становится true)
            if (objects->player->isMustBeDeleted()) {
                objects->showGameOverScreen();
                // Останавливаем обновление игрового мира
                // isGamePaused = true; // не ставим паузу, чтобы кнопка могла обрабатываться
            }
        }
        
        // Если игра не в состоянии gameOver, обновляем объекты
        if (!objects->gameOver) {
            updateObjects(event);
            resetElapsedTime();
        }
    }
}

void GameState::updateObjects(Event event)
{
    for (const auto &object: objects->all) {
        object->update(event);
    }
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

