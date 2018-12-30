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
        updateObjects();
        resetElapsedTime();
    }
}

void GameState::updateObjects()
{
    for (unsigned i = 0; i < objects->all.size(); i++) {
        if (Unit* shooter = dynamic_cast<Unit*>(objects->all[i])) {
            if (shooter->remove()) {
                objects->all.push_back(objects->fabric->createAmmo(
                            shooter->getX(),
                            shooter->getY(),
                            50,
                            50
                            )
                        );
                objects->all.erase(objects->all.begin() + i);
            }
            shooter->update();
            if (!shooter->isJumping()) {      
                shooter->think();
                shooter->gravitate();
            } 
        } else if (Bullet* bullet = dynamic_cast<Bullet*>(objects->all[i])) {
            if (!bullet->move(10.f, 0.f)) {
                objects->all.erase(objects->all.begin() + i);
            }
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

