#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Shooter.h"
#include "Bullet.h"
#include "../CObject.h"
#include "../Window.h"
#include "../GameObjects.h"

Shooter::Shooter()
{
}

Shooter::Shooter(const Shooter& orig)
{
}

Shooter::Shooter(GameObjects* go, int x, int y, int width, int height) 
: CObject(go, x, y, width, height)
{
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(24, 143, 50, 50));
    sprite->setPosition(x, y);
    
    texture = new sf::Texture();
    if (!texture->loadFromFile("resources/images/OpenGunnerHeroVer2.png")) {
        throw;
        std::cout << "NO image found";
    }
    
    explosion = new sf::Texture();
    if (!explosion->loadFromFile("resources/images/oneshot.png")) {
        throw;
        std::cout << "NO image found";
    }
    
    sprite->setTexture(*texture);
}

Shooter::~Shooter()
{
}

bool Shooter::move(int x, int y)
{
    if (!collideObjectAfterMove(x, y) && !this->y + y < 600) {
        this->x += x;
        this->y += y;
        this->sprite->setPosition(this->x, this->y);

        if (y != 0) {
            isFalling = true;
            isMoving = false;
        } else if (x < 0) {
            direction = "left";
            isMoving = true;
        } else if (x > 0) {
            direction = "right";
            isMoving = true;
        }

        return true;
    } 
    isMoving = false;
    isFalling = false;
    
    return false;
}

sf::Drawable* Shooter::getDrawForm()
{
    return sprite;
}

void Shooter::draw(Window* window, float dt)
{
    if (!isDead) {
        // TODO: seems it can be simplified
        if (isMoving && elapsedTime >= animationTime) {
            std::vector<int> current = this->runSprites[direction][currentFrame];
            sprite->setTextureRect(sf::IntRect(current[0], current[1], current[2], current[3]));
            currentFrame++;
            if (currentFrame >= this->runSprites[direction].size()) {
                currentFrame = 0;
            }
            elapsedTime = 0.0;
        } else if (isFalling && !isMoving) {
            std::vector<int> current = this->jumpSprites[direction];
            sprite->setTextureRect(sf::IntRect(current[0], current[1], current[2], current[3]));
        } else if (!isMoving && !isFalling && elapsedTime >= animationTime) {
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
            isNeedToDie = true;
        }
        
        elapsedTime = 0.0;      
    }
    
    elapsedTime += dt;

    this->CObject::draw(window, dt);
    
}

void Shooter::fire()
{
    // TODO: dry
    if (direction == "right") {
        std::vector<int> coords = {getX() + getWidth() + 50, getY() + 20, 10, 10};
        Bullet* bullet = new Bullet(go, coords[0], coords[1], coords[2], coords[3]);
        bullet->setDirection("right");
        go->bullets.push_back(bullet);
    } else {
        std::vector<int> coords = {getX() - 50, getY() + 20, 10, 10};
        Bullet* bullet = new Bullet(go, coords[0], coords[1], coords[2], coords[3]);
        bullet->setDirection("left");
        go->bullets.push_back(bullet);
    }
}

void Shooter::jump(float x, float y)
{
    isJump = true;
    operations.insert(std::pair<std::string, std::vector<float>>("move", {x, y}));
}

void Shooter::moveLeft(float x, float y)
{
    operations.insert(std::pair<std::string, std::vector<float>>("move", {x, y}));
}

void Shooter::runOperations()
{
    if (!operations.empty()) {
         std::multimap<std::string,std::vector<float>>::iterator operation;
        for ( operation = operations.begin(); operation != operations.end();) {
            if (operation->first == "move") {
                std::vector<float> point= operation->second;
                int x = 0;
                int y = 0;
                if (point[0] > 0) {
                    x += 5;
                } else if (point[0] < 0) {
                    x -= 5;
                }

                if (point[1] > 0) {
                    y += 5;
                } else if (point[1] < 0) {
                    y -= 5;
                }
                move(x, y);

                if (operation->second[0] != 0) {
                    operation->second[0] -= x;
                } 

                if (operation->second[1] != 0) {
                    operation->second[1] -= y;            
                }

                if (operation->second[0] == 0 && operation->second[1] == 0) {
                    isJump = false;
                    operations.erase(operation++);
                } else {
                    isJump = true;
                    operation++;
                }
            }
        }
    }
   
}