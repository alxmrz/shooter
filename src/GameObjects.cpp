#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include "CObject.h"
#include "objects/units/Unit.h"
#include "objects/backgrounds/Ground.h"
#include "objects/interactive/Ammunition.h"
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

    groundSprite = fabric->createSprite("ground", 0.f, 0.f);
    groundSprite->setTextureRect(sf::IntRect(0, 100, 50, 50));
    
    crystalSprite = fabric->createSprite("crystal", 0.f, 0.f);
    crystalSprite->setScale(0.5f, 0.5f);
    crystalSprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    cristalCountText = fabric->createText("", 0.f, 0.f);
    ammoCountText = fabric->createText("", 0.f, 0.f);
    
    ammoSprite = fabric->createSprite("ammo", 0.f, 0.f);
    ammoSprite->setScale(0.5f, 0.5f);
    ammoSprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    
    grounds = new sf::RenderTexture;
}

GameObjects::~GameObjects()
{
}

void GameObjects::clear()
{
    buttons.clear();
    all.clear();
    player = NULL;
}

void GameObjects::draw(Window* window, float dt)
{

    sf::Vector2f backgroundCoords = window->mapPixelToCoords(sf::Vector2i(0, 0));
    CObject currentWindow (backgroundCoords.x, backgroundCoords.y, 900, 600);

    //y must be up 300px, because background images is very low
    backgroundSprite->setPosition(backgroundCoords.x, backgroundCoords.y-300);

    window->draw(*backgroundSprite);
    

    for (unsigned i = 0; i < buttons.size(); i++) {
        buttons[i]->draw(window, dt);
    }
    if (gs->isGameStarted) {
        if (groundsSprite == nullptr) {
            grounds->create(5000, 2500);

            for (unsigned i = 0; i < background.size(); i++) {
                groundSprite->setTextureRect(sf::IntRect(background[i][2], background[i][3], 25, 25));
                groundSprite->setPosition(background[i][0], background[i][1]);
                grounds->draw(*groundSprite);
            }

            grounds->display();
            const sf::Texture& texture = grounds->getTexture();
            
            groundsSprite = new sf::Sprite(texture);
        } 
        
       window->draw(*groundsSprite);
        
        for (unsigned i = 0; i < all.size(); i++) {
            if (currentWindow.collideRect(all[i])) {
                all[i]->draw(window, dt);
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
    
    sf::Vector2f ammoWindowCoords = window->mapPixelToCoords(sf::Vector2i(220, 50));
    sf::Vector2f ammoCountCoords = window->mapPixelToCoords(sf::Vector2i(260, 50));

    cristalCountText->message->setString(std::to_string(player->getCrystals()));
    cristalCountText->message->setPosition(crystalCountCoords.x, crystalCountCoords.y);
    crystalSprite->setPosition(cristalWindowCoords.x, cristalWindowCoords.y);
    
    ammoCountText->message->setString(std::to_string(player->getAmmo()));
    ammoCountText->message->setPosition(ammoCountCoords.x, ammoCountCoords.y);
    ammoSprite->setPosition(ammoWindowCoords.x, ammoWindowCoords.y);


    sf::Vector2f windowCoords = window->mapPixelToCoords(sf::Vector2i(50, 50));
    for (int i = 0; i < player->getHealth(); i++) {
        heartSprite->setPosition(windowCoords.x, windowCoords.y);
        window->draw(*heartSprite);
        windowCoords.x += 20;
    }
        
    
    window->draw(*crystalSprite);
    window->draw(*cristalCountText->message);
    
    window->draw(*ammoSprite);
    window->draw(*ammoCountText->message);
        
    
}