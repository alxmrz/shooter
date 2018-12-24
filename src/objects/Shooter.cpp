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
    bool isMapEnd = this->x+x <= 0 || this->y + y <= 0;
    if (!isMapEnd && !collideObjectAfterMove(x, y) == true) {
        this->x += x;
        this->y += y;
        this->shooterSprite->setPosition(this->x, this->y);
        this->explosionSprite->setPosition(this->x, this->y);
        collectCollidedCrystal();
        
        return true;
    } else if (y == 0.f) {
        velocity = 0;
    }
    
    return false;
}

bool Shooter::collectCollidedCrystal()
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
    setDirection(direction);

    moving = move(velocity, 0.f);
    
    return moving;
}

void Shooter::jump()
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

sf::Drawable* Shooter::getDrawForm()
{
    if (!dead) {
        return shooterSprite;
    } else {
        return explosionSprite;
    }
}

void Shooter::draw(Window* window, float dt)
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

void Shooter::animateRun()
{
    shooterSprite->setTextureRect(this->runSprites[direction][currentFrame]);
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

void Shooter::fire()
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

void Shooter::think()
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

bool Shooter::isNextFalling(std::string direction)
{
    int offsetX = 0;
    if (direction == "left") {
        offsetX = -25;
    } else if (direction == "right") {
        offsetX = 25;
    }
    Shooter fake = Shooter(this->go, getX()+offsetX, getY(), getWidth(), getHeight());
    if (!fake.collideObjectAfterMove(0.f, 5.f)) {
        velocity = 0;
        return true;
    } 
    return false;
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

void Shooter::setDirection(std::string direction)
{
    this->direction = direction;
}

void Shooter::decreaseHealth()
{
    health--;
}

void Shooter::setMainSprite(sf::Sprite* mainSprite)
{
    shooterSprite = mainSprite;
}

void Shooter::setHeartSprite(sf::Sprite* heartSprite)
{
    this->heartSprite = heartSprite;
}


void Shooter::setExplosionSprite(sf::Sprite* explosionSprite)
{
    this->explosionSprite = explosionSprite;
    this->explosionSprite->setScale(0.5f, 0.5f);
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
