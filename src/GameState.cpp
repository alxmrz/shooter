#include <iostream>
#include <string>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Application.h"
#include "CObject.h"
#include "objects/Shooter.h"
#include "objects/Bullet.h"

GameState::GameState()
{
}

GameState::GameState(Application* app)
{
    this->app = app;
}

GameState::~GameState()
{
}

void GameState::update()
{
    if (isGameStarted) {
        sf::Time elapsed = this->clock.getElapsedTime();
        float dt = elapsed.asMilliseconds() / 1000.f;
        elapsedTime += dt;
        
        gravity();

        //deletes bullets when they should die. Must be moved in separate method (or class?)
        for (unsigned i = 0; i < objects.bullets.size(); i++) {
            Bullet* bullet = (Bullet*) objects.bullets[i];
            if (!bullet->move(10.f, 0.f)) {
                objects.bullets.erase(objects.bullets.begin() + i);
                break;
            }
        }   
        
        objects.player->runOperations();
    }
}

void GameState::gravity()
{
    if (!objects.player->isJump) {
        float y = 0;
        y += elapsedTime * (velocity + elapsedTime * acceleration / 4);
        
        if (objects.player->move(0, y) || objects.player->move(0, 1)) {
            velocity += elapsedTime * acceleration;
        } else {
            velocity = 0.0;
            elapsedTime = 0.0;
        }
    }
    
}