#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Shooter.h"
#include "Crystal.h"
#include "Bullet.h"
#include "../CObject.h"
#include "../Window.h"
#include "../GameObjects.h"
#include "../Fabric.h"
#include "../ui/Text.h"

Shooter::Shooter()
{
}

Shooter::Shooter(const Shooter& orig)
{
}

Shooter::Shooter(GameObjects* go, float x, float y, int width, int height) 
: CObject(go, x, y, width, height)
{
    
}

Shooter::~Shooter()
{
}

bool Shooter::move(float x, float y)
{
    //TODO: very heavy and slow method. Need to make it faster.
    if (!collideObjectAfterMove(x, y) && this->y + y < 600) {
        
        this->x += x;
        this->y += y;
        this->sprite->setPosition(this->x, this->y);
        collectCollidedCrystal(x, y);
        if (y != 0.f) {
            if (y < 0) {
                jumping = true;
                falling = false;
            } else {
                jumping = false;
                falling = true;
            }
            moving = false;
        } else if (x < 0) {
            direction = "left";
            moving = true;
        } else if (x > 0) {
            direction = "right";
            moving = true;
        }

        return true;
    } 
    if (x != 0.f) {
        moving = false;
    }
    if (y < 0) {
        jumping = false;
    } else if (y > 0) {
        falling = false;
    }
    
    return false;
}

bool Shooter::collectCollidedCrystal(float x, float y)
{
    for (unsigned i = 0; i < go->crystals.size(); i++) {
        Crystal* obj = static_cast<Crystal*>(go->crystals[i]);

        if (collideRect(obj)) {
            crystals++;
            crystalSound->play();
            go->crystals.erase(go->crystals.begin() + i);

            return true;
        }
    }

    return false;
}

bool Shooter::move(std::string direction) 
{
    if (direction == "left") {
        if (velocity > -maxVelocity) {
            velocity -= acceleration;
        }
    } else if (direction == "right") {
        if (velocity < maxVelocity) {
            velocity += acceleration;
        }
    }
    
    return move(velocity, 0.f);
}

sf::Drawable* Shooter::getDrawForm()
{
    return sprite;
}

void Shooter::draw(Window* window, float dt)
{
    // TODO: Need to refactor and simplify
    if (!dead) {
        if (moving && !jumping && !falling && elapsedTime >= animationTime) {
            std::vector<int> current = this->runSprites[direction][currentFrame];
            sprite->setTextureRect(sf::IntRect(current[0], current[1], current[2], current[3]));
            currentFrame++;
            if (currentFrame >= this->runSprites[direction].size()) {
                currentFrame = 0;
            }
            elapsedTime = 0.0;
        } else if ((falling || jumping) && !moving) {
            std::vector<int> current = this->jumpSprites[direction];
            sprite->setTextureRect(sf::IntRect(current[0], current[1], current[2], current[3]));
        } else if (!moving && !falling && !jumping) {
            std::vector<int> current = this->noMotionSprites[direction];
            sprite->setTextureRect(sf::IntRect(current[0], current[1], current[2], current[3]));
        }
    }  else if (elapsedTime >= animationTime){
        sprite = explosionSprite;
        sprite->setScale(0.5f, 0.5f);
        std::vector<int> current = this->explosionSprites[currentFrame];
        sprite->setTextureRect(sf::IntRect(current[0], current[1], current[2], current[3]));
        currentFrame++;
        if (currentFrame >= this->explosionSprites.size()) {
            currentFrame = 0;
            mustBeDeleted = true;
        }
        
        elapsedTime = 0.0;      
    }
    
    elapsedTime += dt;
   
    //TODO: in theory fireTime should not be increased in the function. 
    fireTime += dt;
    gravitationalTime += dt;
    
    if (health > 0 && !main) {
        for (int x = this->x, i = 0; i < health; i++) {
            heartSprite->setPosition(x, y - 25);
            window->draw(*heartSprite);
            x += 20;
        }  
    }
    
    this->CObject::draw(window, dt);
    
}

void Shooter::fire()
{
    if (fireTime >= 0.5f) {
        shotgunSound->play();
        fireTime = 0.f;
        // TODO: dry
        if (direction == "right") {
            std::vector<float> coords = {getX() + getWidth() + 20.f, getY() + 10.f, 10, 10};
            Bullet* bullet = go->fabric->createBullet(coords[0], coords[1], coords[2], coords[3]);
            bullet->setDirection("right");
            go->bullets.push_back(bullet);
        } else {
            std::vector<float> coords = {getX() - 20.f, getY() + 20.f, 10, 10};
            Bullet* bullet = go->fabric->createBullet(coords[0], coords[1], coords[2], coords[3]);
            bullet->setDirection("left");
            go->bullets.push_back(bullet);
        }
    }
}

void Shooter::jump()
{
    if (!isFalling()) {
        if (getVelocityHorizontal() > -getMaxVelocity()) {
            velocityHorizontal -= acceleration;
        }

        if (currentJumpHeight == 0.f) {
            jumpSound->play();
            currentJumpHeight = this->y - 125.f;
        }
        
        jumping = true;

        if (this->y > currentJumpHeight) {
            move(0.f, velocityHorizontal);
        } else {
            falling = true;
            jumping = false;
            currentJumpHeight = 0.f;
        }
    }
}

void Shooter::stopJumping()
{
    jumping = false;
    velocityHorizontal = 0.f;
}

void Shooter::stopMoving()
{
    moving = false;
    velocity = 0.f;
}

void Shooter::gravitate()
{
    float y = 0.f;
    y += gravitationalTime * (gravitationalVelocity + elapsedTime * gravitationalAcceleration / 2.f);

    if (move(0.f, y)) {
        gravitationalVelocity += gravitationalTime * gravitationalAcceleration;
    } else {
        currentJumpHeight = 0.0;
        gravitationalVelocity = 0.0;
        gravitationalTime = 0.0;
    }
}


bool Shooter::collideObjectAfterMove(float x, float y)
{
    x = std::ceil(x);
    y = std::ceil(y);
    float nearX;
    float nearY;
    if (x <= 0.f ) {
         nearX = nearTopLeft(this->x + x);  
    } else if (x > 0.f) {
         nearX = nearDownRight(this->x + x);      
    } 
    
    if (y <= 0.f) {
        nearY = nearTopLeft(this->y + y);
    } else if (y > 0.f) {
        nearY = nearDownRight(this->y + y);
    }

    if (go->backgrounds[nearX][nearY] != nullptr) {
        CObject* nearest = go->backgrounds[nearX][nearY];

        CObject collider = CObject(
                getX() + x,
                getY() + y,
                getWidth(),
                getHeight()
                );
        if (collider.collideRect(nearest)) {
            return true;
        }       
    }

    return false;
}

int Shooter::nearTopLeft(float current)
{
    int decimalResidue = std::floor(current / 10.f) * 10; // 270
    if (decimalResidue % 50 != 0) {
        int hundredPart = std::floor(current / 100.f) * 100; // 200
        int diff = decimalResidue - hundredPart; //70
        if (diff >= 50) {
            return hundredPart + 50;
        } else {
            return hundredPart;
        }
    } else {
        return decimalResidue;
    }
}

int Shooter::nearDownRight(int current)
{
    int decimalResidue = current % 100;
    if (decimalResidue > 50) {
        return std::ceil(current / 100.f) * 100; // 200
    } else {
        return std::floor(current / 100.f) * 100 + 50;
    }
}

bool Shooter::isMoving()
{
    return moving;
}

bool Shooter::isJumping()
{
    return jumping;
}

bool Shooter::isDead()
{
    return dead;
}

bool Shooter::remove()
{
    return mustBeDeleted;
}

bool Shooter::isFalling()
{
    return falling;
}

bool Shooter::isPlayer()
{
    return main;
}

int Shooter::getHealth()
{
    return health;
}

int Shooter::getCrystals()
{
    return crystals;
}

float Shooter::getAcceleration()
{
    return acceleration;
}

float Shooter::getVelocity()
{
    return velocity;
}

float Shooter::getVelocityHorizontal()
{
    return velocityHorizontal;
}

float Shooter::getMaxVelocity()
{
    return maxVelocity;
}

float Shooter::getCurrentJumpHeight()
{
    return currentJumpHeight;
}

void Shooter::setMain(bool main)
{
    this->main = main;
}

void Shooter::setDead(bool dead)
{
    this->dead = dead;
}

void Shooter::decreaseHealth()
{
    health--;
}

void Shooter::setMainSprite(sf::Sprite* mainSprite)
{
    sprite = mainSprite;
}

void Shooter::setHeartSprite(sf::Sprite* heartSprite)
{
    this->heartSprite = heartSprite;
}


void Shooter::setExplosionSprite(sf::Sprite* explosionSprite)
{
    this->explosionSprite = explosionSprite;
}

void Shooter::setJumpSound(sf::Sound* jumpSound)
{
    this->jumpSound = jumpSound;
}

void Shooter::setCrystalSound(sf::Sound* crystalSound)
{
    this->crystalSound = crystalSound;
}

void Shooter::setShotgunSound(sf::Sound* shotgunSound)
{
    this->shotgunSound = shotgunSound;
}
