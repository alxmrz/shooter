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
    //sf::Time elapsed = this->clock.getElapsedTime();
    gravity();
    
    //deletes bullets when they should die. Must be moved in separate method (or class?)
    for (unsigned i = 0; i < objects.bullets.size(); i++) {
        Bullet* bullet = (Bullet*) objects.bullets[i];
        if (!bullet->move(10, 0)) {
            objects.bullets.erase(objects.bullets.begin() + i);
            break;
        }
    }
}

void GameState::gravity()
{
    objects.player->move(0, 5);
}