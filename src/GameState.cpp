#include <iostream>
#include <string>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "GameObjects.h"
#include "Application.h"
#include "CObject.h"
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
    delete backgroundLoop;
    objects->reset();
    delete objects;
}

void GameState::update()
{
    if (isGameStarted) {
        sf::Time elapsed = this->clock.getElapsedTime();
        float dt = elapsed.asMilliseconds() / 1000.f;
        elapsedTime += dt;
        
        gravity();

        //deletes bullets when they should die. Must be moved in separate method (or class?)
        for (unsigned i = 0; i < objects->bullets.size(); i++) {
            Bullet* bullet = (Bullet*) objects->bullets[i];
            if (!bullet->move(10.f, 0.f)) {
                objects->bullets.erase(objects->bullets.begin() + i);
                break;
            }
        }   
        
        objects->player->runOperations();
    }
}

void GameState::gravity()
{
    if (!objects->player->isJump) {
        float y = 0.f;
        y += elapsedTime * (velocity + elapsedTime * acceleration / 2.f);
        
        if (objects->player->move(0.f, y) || objects->player->move(0.f, 2.f)) {
            velocity += elapsedTime * acceleration;
        } else {
            velocity = 0.0;
            elapsedTime = 0.0;
        }
    }
    
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