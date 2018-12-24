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

Bullet::Bullet(Shooter* shooter, GameObjects* go, int x, int y, int width, int height)
: CObject(go, x, y, width, height)
{
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(0, 0, 18, 7));
    sprite->setPosition(x, y);
    this->shooter = shooter;
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

    bool isEndPosition = endPosition[0] == this->x + x && endPosition[1] == this->y + y;
    
    if (!isEndPosition && !collidePlayableAfterMove(x, y) && !collideObjectAfterMove(x, y)) {
        this->x += x;
        this->y += y;
        this->sprite->setPosition(this->x, this->y);

        return true;
    }

    return false;
}

void Bullet::setDirection(std::string direction)
{
    if (direction == "left") {
        /**
         * In constructor we make endPosition.push_back(x + 500); 
         * So, to make a left border for bullet we should make this.
         * It is a hack, so TODO: remake it
         */
        endPosition[0] -= 1000;
        sprite->rotate(180);
    }
    this->direction = direction;
}

bool Bullet::collidePlayableAfterMove(float x, float y)
{
    CObject collider = CObject(
        getX() + x,
        getY() + y,
        getWidth(),
        getHeight()
        );
    for (unsigned i = 0; i < go->playable.size(); i++) {
        Shooter* shooter = static_cast<Shooter*>(go->playable[i]); 

        /** if the player is shot down or the player did the shot */
        bool playerEnvolved = shooter->isPlayer() || this->shooter->isPlayer();
        
        if (playerEnvolved && collider.collideRect(shooter)) { 
            explosionSound->play();

            shooter->decreaseHealth();
            if (shooter->getHealth() == 0) {
                shooter->setDead(true);
            } 

            return true;        
        } 
    }

    return false;
}

void Bullet::setExplosionSound(sf::Sound* sound)
{
    explosionSound = sound;
}

void Bullet::setMainTexture(sf::Texture* texture)
{
    this->sprite->setTexture(*texture);
}