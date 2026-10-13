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

constexpr float Shooter::FIRE_INTERVAL;
constexpr float Shooter::MIN_AIM_LENGTH;
constexpr float Shooter::BULLET_SPAWN_OFFSET;
constexpr int Shooter::BULLET_WIDTH;
constexpr int Shooter::BULLET_HEIGHT;

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
    if (!canFire()) {
        return;
    }

    playShotgunSound();
    fireTime = 0.f;
    if (isPlayer()) {
        --ammo;
    }
    fireBullet();
}

bool Shooter::canFire()
{
    return fireTime >= FIRE_INTERVAL && getAmmo() > 0;
}

void Shooter::playShotgunSound()
{
    if (shotgunSound == nullptr) {
        return;
    }

    shotgunSound->play();
}

void Shooter::fireBullet()
{
    sf::Vector2f direction = firingDirection();
    sf::Vector2f spawn = bulletSpawn(direction);
    Bullet* bullet = createBullet(spawn.x, spawn.y, BULLET_WIDTH, BULLET_HEIGHT);
    bullet->setFlightDirection(direction);
    go->all.push_back(bullet);
}

Bullet* Shooter::createBullet(float x, float y, int width, int height)
{
    return go->fabric->createBullet(this, x, y, width, height);
}

sf::Vector2f Shooter::firingDirection()
{
    if (aimTargetSet) {
        return aimDirectionOrFallback();
    }

    return facingDirection();
}

sf::Vector2f Shooter::aimDirectionOrFallback()
{
    sf::Vector2f center = shooterCenter();
    sf::Vector2f delta(aimTarget.x - center.x, aimTarget.y - center.y);
    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    if (length < MIN_AIM_LENGTH) {
        return facingDirection();
    }

    return sf::Vector2f(delta.x / length, delta.y / length);
}

sf::Vector2f Shooter::facingDirection()
{
    if (direction == "left") {
        return sf::Vector2f(-1.f, 0.f);
    }

    return sf::Vector2f(1.f, 0.f);
}

sf::Vector2f Shooter::shooterCenter()
{
    return sf::Vector2f(getX() + getWidth() / 2.f, getY() + getHeight() / 2.f);
}

sf::Vector2f Shooter::bulletSpawn(sf::Vector2f direction)
{
    sf::Vector2f center = shooterCenter();

    return sf::Vector2f(
        center.x + direction.x * BULLET_SPAWN_OFFSET,
        center.y + direction.y * BULLET_SPAWN_OFFSET
    );
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
