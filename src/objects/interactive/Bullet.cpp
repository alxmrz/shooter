#include <string>
#include <iostream>
#include <SFML/Audio.hpp>
#include "Bullet.h"
#include "../units/Unit.h"
#include "../../GameObjects.h"
#include "../../Window.h"

#include "../../Event.h"


constexpr float Bullet::BULLET_SPEED;
constexpr int Bullet::EXPLOSION_FRAME_DELAY;

Bullet::Bullet()
{
}

Bullet::Bullet(const Bullet& orig)
{
}

Bullet::~Bullet()
{
    delete explosionSprite;
}

Bullet::Bullet(Unit* shooter, GameObjects* go, int x, int y, int width, int height)
: CObject(go, x, y, width, height)
{
    this->shooter = shooter;
    endPosition.push_back(x + 500);
    endPosition.push_back(y);
}

void Bullet::update(Event event)
{ 
    if (exploding) {
        updateExplosion();
        return;
    }

    updateFlight();
}

void Bullet::updateExplosion()
{
    if (!shouldAdvanceExplosion()) {
        return;
    }

    advanceExplosion();
}

bool Bullet::shouldAdvanceExplosion()
{
    explosionTick++;

    return explosionTick >= EXPLOSION_FRAME_DELAY;
}

void Bullet::advanceExplosion()
{
    explosionTick = 0;
    explosionFrame++;

    if (explosionFrame >= explosionFrames.size()) {
        destroy();
        return;
    }

    applyExplosionFrame();
}

void Bullet::applyExplosionFrame()
{
    if (explosionSprite == nullptr) {
        return;
    }

    explosionSprite->setTextureRect(explosionFrames[explosionFrame]);
}

void Bullet::updateFlight()
{
    if (!move(BULLET_SPEED, 0.f)) {
        startExplosion();
    }
}

void Bullet::startExplosion()
{
    if (exploding) {
        return;
    }

    exploding = true;
    explosionFrame = 0;
    explosionTick = 0;
    playExplosionSound();
    positionExplosionSprite();
    applyExplosionFrame();
}

void Bullet::playExplosionSound()
{
    explosionSoundPlays++;

    if (explosionSound == nullptr) {
        return;
    }

    explosionSound->play();
}

void Bullet::positionExplosionSprite()
{
    if (explosionSprite == nullptr) {
        return;
    }

    explosionSprite->setPosition(this->x, this->y);
}

void Bullet::draw(Window* window, float dt)
{
    if (!exploding) {
        CObject::draw(window, dt);
        return;
    }

    window->draw(*explosionSprite);
}

bool Bullet::isExploding()
{
    return exploding;
}

unsigned Bullet::getExplosionFrame()
{
    return explosionFrame;
}

int Bullet::getExplosionSoundPlays()
{
    return explosionSoundPlays;
}

sf::Sprite* Bullet::getExplosionSprite()
{
    return explosionSprite;
}

bool Bullet::move(float x, float y)
{
    if ((direction == "right" && x < 0 )|| direction == "left") {
        x *= -1;
    }

    bool isEndPosition = isEndPositionReached(this->x + x);
    
    if (!isEndPosition && !collidePlayableAfterMove(x, y) && !collideObjectAfterMove(x, y)) {
        this->x += x;
        this->y += y;
        this->mainSprite->setPosition(this->x, this->y);

        return true;
    }

    return false;
}

bool Bullet::isEndPositionReached(float nextX)
{
    if (direction == "left") {
        return nextX <= endPosition[0];
    }

    return nextX >= endPosition[0];
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
        mainSprite->rotate(180);
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
    for (unsigned i = 0; i < go->all.size(); i++) {
        if (Unit* shooter = dynamic_cast<Unit*>(go->all[i])) {
            /** if the player is shot down or the player did the shot */
           bool playerEnvolved = shooter->isPlayer() || this->shooter->isPlayer();

           if (playerEnvolved && collider.collideRect(shooter)) { 
               shooter->decreaseHealth();
               if (shooter->getHealth() == 0) {
                   shooter->setDead(true);
               } 

               return true;        
           }            
        } 
    }

    return false;
}

void Bullet::setExplosionSprite(sf::Sprite* explosionSprite)
{
    this->explosionSprite = explosionSprite;

    if (this->explosionSprite == nullptr) {
        return;
    }

    this->explosionSprite->setScale(0.5f, 0.5f);
    this->explosionSprite->setTextureRect(explosionFrames[0]);
}

void Bullet::setExplosionSound(sf::Sound* sound)
{
    explosionSound = sound;
}
