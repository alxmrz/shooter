#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Unit.h"
#include "../interactive/Crystal.h"
#include "../interactive/Heart.h"
#include "../interactive/Ammunition.h"
#include "../interactive/Platform.h"
#include "../interactive/Bullet.h"
#include "../interfaces/Movable.h"
#include "../../CObject.h"
#include "../../Window.h"
#include "../../GameObjects.h"
#include "../../Fabric.h"
#include "../../ui/Text.h"

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

void Unit::attack()
{
}

void Unit::shiftSpritePositions()
{
}

sf::Drawable* Unit::getDrawForm()
{
    if (!dead) {
        return mainSprite;
    } else {
        return explosionSprite;
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
            attack();
            isMoveLeft = !isMoveRight;
        } else if (isMoveLeft) {
            isMoveLeft = move("left");
            if (isMoveLeft) {
                isMoveLeft = !isNextFalling("left");
            }
            attack();
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

        if (isPlayer()) {
            for (unsigned i = 0; i < go->all.size(); i++) {
                if (Crystal* obj = dynamic_cast<Crystal*> (go->all[i])) {
                   if (collideRect(obj)) {
                    crystals++;
                    crystalSound->play();
                    go->all.erase(go->all.begin() + i);

                    break;
                    } 
                } else if (Ammunition* obj = dynamic_cast<Ammunition*>(go->all[i])) {
                    if (collideRect(obj) && getAmmo() < 10) {
                        ammo += 3;
                        if (ammo > 10) ammo = 10;
                        go->all.erase(go->all.begin() + i);

                        break;
                    }
                } else if (Heart* obj = dynamic_cast<Heart*>(go->all[i])) {
                    if (collideRect(obj) && getHealth() < 3) {
                        health++;
                        go->all.erase(go->all.begin() + i);

                        break;
                    } 
                } else if (Platform* obj = dynamic_cast<Platform*>(go->all[i])) {
                    if (collideRect(obj)) {
                        isPlatformAffected = true;
                        gravitationalVelocity = 0.0;
                        gravitationalTime = 0.0;
                        currentJumpHeight = this->y - 300.f;
                        jump();

                        break;
                    }
                }
            }
        }
        
        return true;
    }
    
    return false;
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

void Unit::update()
{
    if (isPlayer()) {
        if (isPlatformAffected) {
            jump();
            if (!isJumping()) {
                isPlatformAffected = false;
            }
        }       
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

int Unit::getAmmo()
{
    return ammo;
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
    this->mainSprite = mainSprite;
}

void Unit::setHeartSprite(sf::Sprite* heartSprite)
{
    this->heartSprite = heartSprite;
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
