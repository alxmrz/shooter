#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
}

Bullet::Bullet(GameObjects* go, sf::Texture* texture, int x, int y, int width, int height)
: CObject(go, x, y, width, height)
{
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(0, 0, 18, 7));
    sprite->setPosition(x, y);

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
    bool isEndPosition = endPosition[0] == this->x + x && endPosition[1] == this->y + y;
    
    if (!isEndPosition && !collidePlayableAfterMove(x, y) && !collideObjectAfterMove(x, y)) {
        this->x += x;
        this->y += y;
        this->sprite->setPosition(this->x, this->y);

        return true;
    }

    return false;
}

void Bullet::setDirection(std::string dir)
{
    if (dir == "left") {
        //TODO: it is a hack to set left border for bullet. @see constructor
        endPosition[0] -= 1000;
        sprite->rotate(180);
    }
    direction = dir;
}

bool Bullet::collidePlayableAfterMove(float x, float y)
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
            explosionSound->play();
            ((Shooter*)go->playable[i])->health--;
            if (((Shooter*)go->playable[i])->health == 0) {
                ((Shooter*)go->playable[i])->isDead = true;
            } 
            
            delete collider;
            return true;
        }

        delete collider;
    }

    return false;
}

void Bullet::setExplosionSound(sf::Sound* sound)
{
    explosionSound = sound;
}