#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "Shooter.h"
#include "../GameObjects.h"

Bullet::Bullet()
{
}

Bullet::Bullet(const Bullet& orig)
{
}

Bullet::~Bullet()
{
    delete sprite;
    delete texture;
}

Bullet::Bullet(GameObjects* go, int x, int y, int width, int height)
: CObject(go, x, y, width, height)
{
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(0, 0, 18, 7));
    sprite->setPosition(x, y);
    texture = new sf::Texture();
    if (!texture->loadFromFile("resources/images/shell.png")) {
        std::cout << "Image images/ground.jpeg is not found" << std::endl;
        throw;
    }
    sprite->setTexture(*texture);

    endPosition.push_back(x + 500);
    endPosition.push_back(y);
}

sf::Drawable* Bullet::getDrawForm()
{
    return sprite;
}

bool Bullet::move(float x, float y)
{
    if ((direction == "right" && x < 0 )|| direction == "left") {
        x *= -1;
    }

    // TODO: isEndPosition is not in use yet. Fix and use.
    //bool isEndPosition = endPosition[0] == this->x + x && endPosition[1] == this->y + y;
    
    if (!collidePlayableAfterMove(x, y) && !collideObjectAfterMove(x, y)) {
        this->x += x;
        this->y += y;
        this->sprite->setPosition(this->x, this->y);

        return true;
    }

    return false;
}

void Bullet::setDirection(std::string dir)
{
    direction = dir;
}

bool Bullet::collidePlayableAfterMove(int x, int y)
{
    for (unsigned i = 0; i < go->playable.size(); i++) {
        auto* obj = go->playable[i];
        CObject* collider = new CObject(
                getX() + x,
                getY() + y,
                getWidth(),
                getHeight()
                );
        if (collider->collideRect(obj)) {
            ((Shooter*)go->playable[i])->isDead = true;
            delete collider;
            return true;
        }

        delete collider;
    }

    return false;
}