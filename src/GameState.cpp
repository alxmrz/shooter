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
    //sf::Time elapsed = this->clock.getElapsedTime();
    gravity();
    
    
}

void GameState::gravity()
{
    sf::Time elapsed = clock.getElapsedTime();
}