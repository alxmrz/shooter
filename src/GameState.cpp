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
    objects = new GameObjects(this);
    backgroundMusic = objects->fabric->getBackgroundSound();
}

GameState::~GameState()
{
    objects->reset();
    delete backgroundMusic;
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
    for (auto* obj: objects->playable) {
        Shooter* shooter = static_cast<Shooter*>(obj);
        if (!shooter->isJumping()) {
            shooter->think();
            shooter->gravitate();
        } 
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

