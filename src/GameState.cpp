#include <iostream>
#include <string>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Application.h"
#include "CObject.h"
#include "Builder.h"
#include "objects/Shooter.h"

GameState::GameState() {
}

GameState::GameState(Application* app) {
    this->app = app;
    
}

GameState::~GameState() {
}

void GameState::update()
{
    sf::Time elapsed = this->clock.getElapsedTime();
    gravity();
    float seconds = elapsed.asSeconds();
    if (seconds >= 1) {
       this->clock.restart();
       this->frame = 0;
    } else {
       this->frame++; 
    }
}

void GameState::gravity()
{
   
    bool collided = false;
    for (auto* obj: objects.background)
    {
        CObject* collider = new CObject(
                objects.player->getX(), 
                objects.player->getY()+10, 
                objects.player->getWidth(), 
                objects.player->getHeight()
        );
        if (collider->collideRect(obj))
        {
            collided = true;
            break;
        }
        delete collider;
    }
    
    if (!collided) {
        objects.player->move(0, 1); 
    }
    
}