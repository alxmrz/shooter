#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "GameObjects.h"
#include "Application.h"
#include "CObject.h"
#include "Scene.h"
#include "objects/Shooter.h"
#include "objects/Bullet.h"
#include "Fabric.h"

GameState::GameState()
{
}

GameState::GameState(Application* app)
{
    this->app = app;
    objects = new GameObjects();
    backgroundLoop = objects->fabric->getBackgroundSound();
}

GameState::~GameState()
{
    objects->reset();
    delete backgroundLoop;
    delete objects;
}

void GameState::update()
{
    if (isGameStarted && !isGamePaused) {
        updateBulletsState();
        causeGravity();
        resetElapsedTime();
    }
}


void GameState::updateBulletsState()
{
    for (unsigned i = 0; i < objects->bullets.size(); i++) {
        Bullet* bullet = static_cast<Bullet*>(objects->bullets[i]);
        if (!bullet->move(10.f, 0.f)) {
            objects->bullets.erase(objects->bullets.begin() + i);
            break;
        }
    }   
}

void GameState::causeGravity()
{
    if (!objects->player->isJumping()) {
        objects->player->gravitate();
    }
}

void GameState::resetElapsedTime()
{
    sf::Time elapsed = this->clock.getElapsedTime();
    elapsedTime = elapsed.asMilliseconds() / 1000.f;
}

void GameState::playBackgroundSound()
{
    backgroundLoop->setLoop(true);
    backgroundLoop->play();
}

void GameState::stopBackgroundSound()
{
    backgroundLoop->setLoop(false);
    backgroundLoop->stop();
}

void GameState::startNewGame()
{
    isGameStarted = true;
    playBackgroundSound();
    app->scene->initNewGame();
}

float GameState::getElapsedTime()
{
    return elapsedTime;
}

