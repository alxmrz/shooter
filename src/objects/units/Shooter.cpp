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

Shooter::Shooter(GameObjects* go, float x, float y, int width, int height) 
: Unit(go, x, y, width, height)
{
    
}

Shooter::~Shooter()
{
}

void Shooter::destroy()
{
    CObject::destroy();

   /* Ammunition* amma= go->fabric->createAmmo(
                            getX(),
                            getY(),
                            50,
                            50
                            );

    CObject* ammo = dynamic_cast<CObject*>(amma); */

    //go->all.push_back(ammo);
}

void Shooter::draw(Window* window, float dt)
{
    if (go->gameOver) {
        this->CObject::draw(window, dt);
        return;
    }

    if (!dead) {
        refreshSpriteFrame(dt);
    } else if (!mustBeDeleted) {
        animateExplosion(dt);
    }

    if (!(dead && mustBeDeleted)) {
        fireTime += dt;
        gravitationalTime += dt;
    }

    bool aliveNPC = health > 0 && !isPlayer();
    if (aliveNPC) {
        drawHearts(window);
    }

    applyRotation(dt);

    this->CObject::draw(window, dt);
}

void Shooter::refreshSpriteFrame(float dt)
{
    elapsedTime += dt;

    if (isAirborne()) {
        refreshJumpFrame();
        return;
    }

    refreshGroundFrame();
}

bool Shooter::isAirborne()
{
    return falling || jumping;
}

void Shooter::refreshGroundFrame()
{
    if (moving) {
        refreshMovingFrame();
        return;
    }

    refreshIdleFrame();
}

void Shooter::refreshMovingFrame()
{
    if (elapsedTime >= animationTime) {
        advanceRunFrame();
    }

    showRunFrame();
}

void Shooter::advanceRunFrame()
{
    currentFrame++;

    if (currentFrame >= this->runSprites[direction].size()) {
        currentFrame = 0;
    }

    elapsedTime = 0.0;
}

void Shooter::showRunFrame()
{
    mainSprite->setTextureRect(this->runSprites[direction][currentFrame]);
}

void Shooter::refreshJumpFrame()
{
    mainSprite->setTextureRect(this->jumpSprites[direction]);
}

void Shooter::refreshIdleFrame()
{
    mainSprite->setTextureRect(this->noMotionSprites[direction]);
}

void Shooter::animateExplosion(float dt)
{
    elapsedTime += dt;

    if (elapsedTime >= animationTime) {
        showNextExplosionFrame();
    }
}

void Shooter::showNextExplosionFrame()
{
    explosionSprite->setTextureRect(this->explosionSprites[currentFrame]);
    advanceExplosionFrame();
    elapsedTime = 0.0;
}

void Shooter::advanceExplosionFrame()
{
    currentFrame++;

    if (currentFrame >= this->explosionSprites.size()) {
        currentFrame = this->explosionSprites.size() - 1;
        mustBeDeleted = true;
    }
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
    if (fireTime >= 0.5f && getAmmo() > 0) {
        shotgunSound->play();
        fireTime = 0.f;
        if (isPlayer()) {
            --ammo;
        }
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
        go->all.push_back(bullet);
    }
}

void Shooter::shiftSpritePositions()
{
    float originX = this->mainSprite ? this->mainSprite->getOrigin().x : 0.f;
    float originY = this->mainSprite ? this->mainSprite->getOrigin().y : 0.f;
    this->mainSprite->setPosition(this->x + originX, this->y + originY);
    this->explosionSprite->setPosition(this->x, this->y);
}


void Shooter::setMainSprite(sf::Sprite* mainSprite)
{
    Unit::setMainSprite(mainSprite);

    if (this->mainSprite) {
        this->mainSprite->setOrigin(this->getWidth() / 2.f, this->getHeight() / 2.f);
        this->mainSprite->setPosition(
            this->x + this->mainSprite->getOrigin().x,
            this->y + this->mainSprite->getOrigin().y
        );
        this->mainSprite->setTextureRect(this->noMotionSprites[this->direction]);
    }
}


void Shooter::setExplosionSprite(sf::Sprite* explosionSprite)
{
    this->explosionSprite = explosionSprite;
    this->explosionSprite->setScale(0.5f, 0.5f);
    explosionSprite->setTextureRect(this->explosionSprites[0]);
}

void Shooter::startRotation()
{
    rotating = true;
    rotationAngle = 0.f;
}

void Shooter::onSecondJump()
{
    startRotation();
}

void Shooter::applyRotation(float dt)
{
    if (!rotating) {
        return;
    }
    
    // Rotation speed: 360 degrees per 0.5 seconds
    float rotationSpeed = 360.f / 0.5f; // degrees per second
    rotationAngle += rotationSpeed * dt;
    
    if (rotationAngle >= 360.f) {
        rotationAngle = 0.f;
        rotating = false;
    }
    
    // Apply rotation to main sprite
    if (mainSprite) {
        mainSprite->setRotation(rotationAngle);
    }
}
