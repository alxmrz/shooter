#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Unit.h"
#include "Crystal.h"
#include "Bullet.h"
#include "Movable.h"
#include "../CObject.h"
#include "../Window.h"
#include "../GameObjects.h"
#include "../Fabric.h"
#include "../ui/Text.h"

Unit::Unit()
{
}

Unit::Unit(const Unit& orig)
{
}

Unit::Unit(GameObjects* go, float x, float y, int width, int height) 
: Movable(go, x, y, width, height)
{
    
}

Unit::~Unit()
{
}

bool Unit::collectCollidedCrystal()
{
    if (isPlayer()) {
        for (unsigned i = 0; i < go->crystals.size(); i++) {
            Crystal* obj = static_cast<Crystal*> (go->crystals[i]);

            if (collideRect(obj)) {
                crystals++;
                crystalSound->play();
                go->crystals.erase(go->crystals.begin() + i);

                return true;
            }
        }   
    }
    return false;
}

sf::Drawable* Unit::getDrawForm()
{
    if (!dead) {
        return shooterSprite;
    } else {
        return explosionSprite;
    }
}

void Unit::draw(Window* window, float dt)
{
    bool showNextFrame = elapsedTime >= animationTime;
    if (!dead) {
        if (falling || jumping) {
            shooterSprite->setTextureRect(this->jumpSprites[direction]);
        } else if (moving && showNextFrame) {
            animateRun();
        } else if (!moving && !falling && !jumping) {
            shooterSprite->setTextureRect(this->noMotionSprites[direction]);
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

void Unit::animateRun()
{
    shooterSprite->setTextureRect(this->runSprites[direction][currentFrame]);
    currentFrame++;
    if (currentFrame >= this->runSprites[direction].size()) {
        currentFrame = 0;
    }
    elapsedTime = 0.0;
}

void Unit::animateExplosion()
{
    explosionSprite->setTextureRect(this->explosionSprites[currentFrame]);
    currentFrame++;
    if (currentFrame >= this->explosionSprites.size()) {
        currentFrame = 0;
        mustBeDeleted = true;
    }

    elapsedTime = 0.0;
}

void Unit::drawHearts(Window* window)
{
    for (int x = this->x, i = 0; i < health; i++) {
        heartSprite->setPosition(x, y - 25);
        window->draw(*heartSprite);
        x += 20;
    }
}

void Unit::fire()
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

void Unit::think()
{
    if (!isPlayer() && !isDead()) {
        if (isMoveRight) {
            isMoveRight = move("right");
            if (isMoveRight) {
                isMoveRight = !isNextFalling("right");
            }
            fire();
            isMoveLeft = !isMoveRight;
        } else if (isMoveLeft) {
            isMoveLeft = move("left");
            if (isMoveLeft) {
                isMoveLeft = !isNextFalling("left");
            }
            fire();
            isMoveRight = !isMoveLeft;
        }
    }
}

bool Unit::isNextFalling(std::string direction)
{
    int offsetX = 0;
    if (direction == "left") {
        offsetX = -25;
    } else if (direction == "right") {
        offsetX = 25;
    }
    Unit fake = Unit(this->go, getX()+offsetX, getY(), getWidth(), getHeight());
    if (!fake.collideObjectAfterMove(0.f, 5.f)) {
        velocity = 0;
        return true;
    } 
    return false;
}

bool Unit::move(float x, float y) 
{
    if (Movable::move(x, y)) {
        shiftSpritePositions();
        collectCollidedCrystal();
        
        return true;
    }
    
    return false;
}

void Unit::shiftSpritePositions()
{
    this->shooterSprite->setPosition(this->x, this->y);
    this->explosionSprite->setPosition(this->x, this->y);
}

void Unit::jump()
{
    if (velocityHorizontal > -maxVelocity) {
        velocityHorizontal -= acceleration;
    }

    if (currentJumpHeight == 0.f) {
        jumpSound->play();
        currentJumpHeight = this->y - 125.f;
    }

    if (this->y > currentJumpHeight && move(0.f, velocityHorizontal)) {
        jumping = true;
    } else {
        stopJumping();
    }
}

void Unit::gravitate()
{
    float y = std::ceil(
            gravitationalTime * (gravitationalVelocity + elapsedTime * gravitationalAcceleration / 2.f)
            );
    
    falling = y > 0 && move(0.f, y);
    if (falling) {
        gravitationalVelocity += gravitationalTime * gravitationalAcceleration;
    } else {
        currentJumpHeight = 0.0;
        gravitationalVelocity = 0.0;
        gravitationalTime = 0.0;
    }
}

bool Unit::isJumping()
{
    return jumping;
}

bool Unit::isFalling()
{
    return falling;
}

void Unit::stopJumping()
{
    jumping = false;
    velocityHorizontal = 0.f;
}


bool Unit::isDead()
{
    return dead;
}

bool Unit::remove()
{
    return mustBeDeleted;
}

bool Unit::isPlayer()
{
    return main;
}

int Unit::getHealth()
{
    return health;
}

int Unit::getCrystals()
{
    return crystals;
}

float Unit::getVelocityHorizontal()
{
    return velocityHorizontal;
}

float Unit::getCurrentJumpHeight()
{
    return currentJumpHeight;
}

void Unit::setMain(bool main)
{
    this->main = main;
}

void Unit::setDead(bool dead)
{
    this->dead = dead;
}

void Unit::decreaseHealth()
{
    health--;
}

void Unit::setMainSprite(sf::Sprite* mainSprite)
{
    shooterSprite = mainSprite;
}

void Unit::setHeartSprite(sf::Sprite* heartSprite)
{
    this->heartSprite = heartSprite;
}


void Unit::setExplosionSprite(sf::Sprite* explosionSprite)
{
    this->explosionSprite = explosionSprite;
    this->explosionSprite->setScale(0.5f, 0.5f);
    explosionSprite->setTextureRect(this->explosionSprites[0]);
}

void Unit::setJumpSound(sf::Sound* jumpSound)
{
    this->jumpSound = jumpSound;
}

void Unit::setCrystalSound(sf::Sound* crystalSound)
{
    this->crystalSound = crystalSound;
}

void Unit::setShotgunSound(sf::Sound* shotgunSound)
{
    this->shotgunSound = shotgunSound;
}
