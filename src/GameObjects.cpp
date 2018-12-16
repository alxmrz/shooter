#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include "CObject.h"
#include "objects/Shooter.h"
#include "ui/Button.h"
#include "Fabric.h"
#include "Window.h"

GameObjects::GameObjects()
{
    fabric = new Fabric(this);
    std::string type = "background";
    backgroundSprite = fabric->createSprite(type, 0.f, 0.f);
}

GameObjects::~GameObjects()
{
}

std::vector<CObject*>* GameObjects::all()
{
    // TODO: fix that. Must return all objects in one container
    
    std::vector<CObject*>* all = {};
    
    return all;
}

void GameObjects::reset()
{
    buttons.clear();
    playable.clear();
    background.clear();
    crystals.clear();
    bullets.clear();
    player = NULL;
}

void GameObjects::draw(Window* window, float dt)
{
    sf::Vector2f backgroundCoords = window->mapPixelToCoords(sf::Vector2i(0, 0));
    //y must be up 300px, because background images is very low
    backgroundSprite->setPosition(backgroundCoords.x, backgroundCoords.y-300);

    window->draw(*backgroundSprite);
    // TODO: this is a bad realization. Need to refactor and simplify
    for (unsigned i = 0; i < buttons.size(); i++) {
        buttons[i]->draw(window, dt);
    }

    for (unsigned i = 0; i < background.size(); i++) {
        background[i]->draw(window, dt);
    }
    for (unsigned i = 0; i < crystals.size(); i++) {
        crystals[i]->draw(window, dt);
    }

    for (unsigned i = 0; i < bullets.size(); i++) {
        bullets[i]->draw(window, dt);
    }
    
    for (unsigned i = 0; i < playable.size(); i++) {
        playable[i]->draw(window, dt);
        if (((Shooter*)playable[i])->mustBeDeleted) {
            playable.erase(playable.begin() + i);
        }
    }
}

void GameObjects::correctBackgroundSprite()
{
    
}