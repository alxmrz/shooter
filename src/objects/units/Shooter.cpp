#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Shooter.h"
#include "../interactive/Crystal.h"
#include "../interactive/Bullet.h"
#include "../interfaces/Movable.h"
#include "../../CObject.h"
#include "../../Window.h"
#include "../../GameObjects.h"
#include "../../Fabric.h"
#include "../../ui/Text.h"

Shooter::Shooter()
{
}

Shooter::Shooter(const Shooter& orig)
{
}

Shooter::Shooter(GameObjects* go, float x, float y, int width, int height) 
: Unit(go, x, y, width, height)
{
    
}

Shooter::~Shooter()
{
}

void Shooter::draw(Window* window, float dt)
{
    bool showNextFrame = elapsedTime >= animationTime;
    if (!dead) {
        if (falling || jumping) {
            mainSprite->setTextureRect(this->jumpSprites[direction]);
        } else if (moving && showNextFrame) {
            animateRun();
        } else if (!moving && !falling && !jumping) {
            mainSprite->setTextureRect(this->noMotionSprites[direction]);
        }
    }  else if (showNextFrame){
        animateExplosion();
    }
    
    elapsedTime += dt;
    fireTime += dt;
    gravitationalTime += dt;
    
    bool aliveNPC = health > 0 && !isPlayer();
    if (aliveNPC) {
        drawHearts(window);
    }
    
    this->CObject::draw(window, dt);
    
}

void Shooter::animateRun()
{
    mainSprite->setTextureRect(this->runSprites[direction][currentFrame]);
    currentFrame++;
    if (currentFrame >= this->runSprites[direction].size()) {
        currentFrame = 0;
    }
    elapsedTime = 0.0;
}

void Shooter::animateExplosion()
{
    explosionSprite->setTextureRect(this->explosionSprites[currentFrame]);
    currentFrame++;
    if (currentFrame >= this->explosionSprites.size()) {
        currentFrame = 0;
        mustBeDeleted = true;
    }

    elapsedTime = 0.0;
}

void Shooter::drawHearts(Window* window)
{
    for (int x = this->x, i = 0; i < health; i++) {
        heartSprite->setPosition(x, y - 25);
        window->draw(*heartSprite);
        x += 20;
    }
}

void Shooter::attack()
{
    if (fireTime >= 0.5f) {
        shotgunSound->play();
        fireTime = 0.f;
        Bullet* bullet;
        std::vector<float> coords;
        if (direction == "right") {
            coords = {getX() + getWidth() + 20.f, getY() + 10.f, 10, 10};
            bullet = go->fabric->createBullet(this, coords[0], coords[1], coords[2], coords[3]);
        } else {
            coords = {getX() - 20.f, getY() + 20.f, 10, 10};
            bullet = go->fabric->createBullet(this, coords[0], coords[1], coords[2], coords[3]);
        }
        bullet->setDirection(direction); 
        go->bullets.push_back(bullet);
    }
}

void Shooter::shiftSpritePositions()
{
    this->mainSprite->setPosition(this->x, this->y);
    this->explosionSprite->setPosition(this->x, this->y);
}


void Shooter::setExplosionSprite(sf::Sprite* explosionSprite)
{
    this->explosionSprite = explosionSprite;
    this->explosionSprite->setScale(0.5f, 0.5f);
    explosionSprite->setTextureRect(this->explosionSprites[0]);
}
