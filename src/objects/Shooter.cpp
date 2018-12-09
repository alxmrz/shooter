#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Shooter.h"
#include "../CObject.h"
#include "../Window.h"
#include "../GameObjects.h"

Shooter::Shooter() {
}

Shooter::Shooter(const Shooter& orig) {
}

Shooter::Shooter(GameObjects* go, int x, int y, int width, int height): CObject(go, x, y, width, height)
{
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(24, 143, 50, 50));
    sprite->setPosition(x,y);
    texture = new sf::Texture();
    if (!texture->loadFromFile("images/OpenGunnerHeroVer2.png"))
    {
        throw;
        std::cout << "NO image found";
    }
    sprite->setTexture(*texture);
}

Shooter::~Shooter() {
}

void Shooter::move(int x, int y)
{    
    if (!collideObjectAfterMove(x, y))
    {
        this->x += x;
        this->y += y;
        this->sprite->setPosition(this->x, this->y);
        
        if (y != 0) 
        {
            isJump = true;
            isMoving = false;
        }
        else if (x < 0)
        {
            direction = "left";
            isMoving = true;
        } 
        else if (x > 0)
        {
            direction = "right";
            isMoving = true;
        }
        
    }
    
    
}

sf::Drawable* Shooter::getDrawForm()
{
    return sprite;
}

void Shooter::draw(Window* window, float dt)
{
    if (isMoving && elapsedTime >= animationTime) 
    {
        std::vector<int> current = this->runSprites[direction][currentFrame];
        sprite->setTextureRect(sf::IntRect(current[0], current[1], current[2], current[3]));
        currentFrame++;
        if (currentFrame >= this->runSprites[direction].size()) 
        {
            currentFrame = 0;
        }
        elapsedTime = 0.0;
    }
    else if (isJump && !isMoving)
    {
        std::vector<int> current = this->jumpSprites[direction];
        sprite->setTextureRect(sf::IntRect(current[0], current[1], current[2], current[3]));
    }
    else if (!isMoving && !isJump && elapsedTime >= animationTime)
    {
        std::vector<int> current = this->noMotionSprites[direction];
        sprite->setTextureRect(sf::IntRect(current[0], current[1], current[2], current[3]));
    }
    elapsedTime += dt;
    
    this->CObject::draw(window, dt);
    
    isMoving = false;
    isJump = false;
}