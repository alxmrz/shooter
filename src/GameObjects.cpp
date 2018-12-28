#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include "CObject.h"
#include "objects/units/Unit.h"
#include "objects/backgrounds/Ground.h"
#include "ui/Button.h"
#include "ui/Text.h"
#include "Fabric.h"
#include "Window.h"
#include "GameState.h"

GameObjects::GameObjects(GameState* gs)
{
    this->gs = gs;
    fabric = new Fabric(this);
    backgroundSprite = fabric->createSprite("background", 0.f, 0.f);
    
    heartSprite = fabric->createSprite("heart", 0.f, 0.f);
    heartSprite->setScale(0.5f, 0.5f);
    heartSprite->setTextureRect(sf::IntRect(0, 0, 50, 50));

    
    crystalSprite = fabric->createSprite("crystal", 0.f, 0.f);
    crystalSprite->setScale(0.5f, 0.5f);
    crystalSprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    text = fabric->createText("", 0.f, 0.f);
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
    if (gs->isGameStarted) {
        for (unsigned i = 0; i < background.size(); i++) {
            background[i]->draw(window, dt);
        }   
        
        for (unsigned i = 0; i < crystals.size(); i++) {
            crystals[i]->draw(window, dt);
        }
        
        for (unsigned i = 0; i < hearts.size(); i++) {
            hearts[i]->draw(window, dt);
        }

        for (unsigned i = 0; i < bullets.size(); i++) {
            bullets[i]->draw(window, dt);
        }

        for (unsigned i = 0; i < playable.size(); i++) {
            playable[i]->draw(window, dt);
            if (((Unit*)playable[i])->remove()) {
                playable.erase(playable.begin() + i);
            }
        }
        drawPlayerUi(window);  
    }
}

void GameObjects::correctBackgroundSprite()
{
    
}

void GameObjects::drawPlayerUi(Window* window)
{
       
    sf::Vector2f cristalWindowCoords = window->mapPixelToCoords(sf::Vector2i(150, 50));
    sf::Vector2f crystalCountCoords = window->mapPixelToCoords(sf::Vector2i(180, 50));

    text->message->setString(std::to_string(player->getCrystals()));
    text->message->setPosition(crystalCountCoords.x, crystalCountCoords.y);
    crystalSprite->setPosition(cristalWindowCoords.x, cristalWindowCoords.y);


    sf::Vector2f windowCoords = window->mapPixelToCoords(sf::Vector2i(50, 50));
    for (int i = 0; i < player->getHealth(); i++) {
        heartSprite->setPosition(windowCoords.x, windowCoords.y);
        window->draw(*heartSprite);
        windowCoords.x += 20;
    }
        
    
    window->draw(*crystalSprite);
    window->draw(*text->message);
        
    
}