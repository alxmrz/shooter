#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Shooter.h"
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
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(24, 143, 50, 50));
    sprite->setPosition(x, y);
    
    heartSprite = new sf::Sprite();
    heartSprite->setScale(0.5f, 0.5f);
    heartSprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    heartSprite->setPosition(x, y);
    
    crystalSprite = new sf::Sprite();
    crystalSprite->setScale(0.5f, 0.5f);
    crystalSprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    crystalSprite->setPosition(x, y);
}

Shooter::~Shooter()
{
}

bool Shooter::move(float x, float y)
{
    if (!collideObjectAfterMove(x, y) && !this->y + y < 600) {
        collectCollidedCrystal(x, y);
        this->x += x;
        this->y += y;
        this->sprite->setPosition(this->x, this->y);
        
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
    if (!dead) {
        // TODO: seems it can be simplified
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
        } else if (!moving && !falling && !jumping && elapsedTime >= animationTime) {
            std::vector<int> current = this->noMotionSprites[direction];
            sprite->setTextureRect(sf::IntRect(current[0], current[1], current[2], current[3]));
        }
    }  else if (elapsedTime >= animationTime){
        sprite->setTexture(*explosion);
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
    fireTime += dt;
    
    if (health > 0) {
        if (!main) {
           for (int x=this->x,i=0; i < health; i++) {
                heartSprite->setPosition(x, y-25);
                window->draw(*heartSprite);
                x+=20;
            } 
        } else {
            sf::Vector2f windowCoords = window->mapPixelToCoords(sf::Vector2i(50, 50));
            for (int i=0; i < health; i++) {
                heartSprite->setPosition(windowCoords.x, windowCoords.y);
                window->draw(*heartSprite);
                windowCoords.x += 20;
            }
        }
        
    }
    
    if (main) {
        sf::Vector2f cristalWindowCoords = window->mapPixelToCoords(sf::Vector2i(150, 50));
        sf::Vector2f crystalCount = window->mapPixelToCoords(sf::Vector2i(180, 50));
        
        text->message->setString(std::to_string(crystals));
        text->message->setPosition(crystalCount.x, crystalCount.y);
        crystalSprite->setPosition(cristalWindowCoords.x, cristalWindowCoords.y);
        
        window->draw(*crystalSprite);
        window->draw(*text->message);
        
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
    y += elapsedTime * (gravitationalVelocity + elapsedTime * gravitationalAcceleration / 2.f);

    if (move(0.f, y) || move(0.f, 2.f)) {
        gravitationalVelocity += elapsedTime * gravitationalAcceleration;
    } else {
        currentJumpHeight = 0.0;
        gravitationalVelocity = 0.0;
        elapsedTime = 0.0;
    }
}

bool Shooter::collectCollidedCrystal(float x, float y)
{
    for (unsigned i = 0; i < go->crystals.size(); i++) {
        auto* obj = go->crystals[i];
        CObject* collider = new CObject(
                getX() + x,
                getY() + y,
                getWidth(),
                getHeight()
                );
        if (collider->collideRect(obj)) {
            crystals++;
            crystalSound->play();
            go->crystals.erase(go->crystals.begin() + i);
            delete collider;
            return true;
        }

        delete collider;
    }

    return false;
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

bool Shooter::isNeedToDie()
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



void Shooter::setMainTexture(sf::Texture* texture)
{
    sprite->setTexture(*texture);
}

void Shooter::setHeartTexture(sf::Texture* texture)
{
    heartSprite->setTexture(*texture);
}
void Shooter::setCrystalTexture(sf::Texture* texture)
{
    crystalSprite->setTexture(*texture);
}

void Shooter::setExplosionTexture(sf::Texture* texture)
{
    explosion = texture;
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

void Shooter::setCrystalCountText(Text* text)
{
    this->text = text;
}