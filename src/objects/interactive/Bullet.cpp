#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <SFML/Audio.hpp>
#include "Bullet.h"
#include "../units/Unit.h"
#include "../../GameObjects.h"
#include "../../Window.h"

#include "../../Event.h"


constexpr float Bullet::BULLET_SPEED;
constexpr float Bullet::MAX_DISTANCE;
constexpr int Bullet::EXPLOSION_FRAME_DELAY;

namespace {
const float MIN_DIRECTION_LENGTH = 0.0001f;
const float RAD_TO_DEG = 57.2957795f;
}

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
    startPosition = sf::Vector2f(x, y);
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
    if (!move(flightDir.x * BULLET_SPEED, flightDir.y * BULLET_SPEED)) {
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
    if (blockedAt(x, y)) {
        return false;
    }

    advance(x, y);
    return true;
}

bool Bullet::blockedAt(float x, float y)
{
    return isMaxDistanceReached(this->x + x, this->y + y) || collidesAt(x, y);
}

bool Bullet::collidesAt(float x, float y)
{
    return collidePlayableAfterMove(x, y) || collideObjectAfterMove(x, y);
}

void Bullet::advance(float x, float y)
{
    this->x += x;
    this->y += y;
    positionSprite();
}

bool Bullet::isMaxDistanceReached(float nextX, float nextY)
{
    float dx = nextX - startPosition.x;
    float dy = nextY - startPosition.y;

    return std::sqrt(dx * dx + dy * dy) >= MAX_DISTANCE;
}

void Bullet::positionSprite()
{
    if (mainSprite == nullptr) {
        return;
    }

    mainSprite->setPosition(this->x + this->width / 2.f, this->y + this->height / 2.f);
}

void Bullet::setDirection(std::string direction)
{
    if (direction == "left") {
        setFlightDirection(sf::Vector2f(-1.f, 0.f));
        return;
    }

    setFlightDirection(sf::Vector2f(1.f, 0.f));
}

void Bullet::setFlightDirection(sf::Vector2f direction)
{
    flightDir = normalizedDirection(direction);
    applySpriteRotation();
}

sf::Vector2f Bullet::normalizedDirection(sf::Vector2f direction)
{
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length < MIN_DIRECTION_LENGTH) {
        return sf::Vector2f(1.f, 0.f);
    }

    return sf::Vector2f(direction.x / length, direction.y / length);
}

void Bullet::applySpriteRotation()
{
    if (mainSprite == nullptr) {
        return;
    }

    float angle = std::atan2(flightDir.y, flightDir.x) * RAD_TO_DEG;
    mainSprite->setRotation(angle);
}

float Bullet::getVelocityX()
{
    return flightDir.x * BULLET_SPEED;
}

float Bullet::getVelocityY()
{
    return flightDir.y * BULLET_SPEED;
}

float Bullet::getRotation()
{
    if (mainSprite == nullptr) {
        return 0.f;
    }

    float angle = mainSprite->getRotation();
    return std::fmod(angle + 180.f, 360.f) - 180.f;
}

bool Bullet::collidePlayableAfterMove(float x, float y)
{
    CObject collider = CObject(
        getX() + x,
        getY() + y,
        getWidth(),
        getHeight()
        );

    return std::any_of(go->all.begin(), go->all.end(), UnitImpactDetector{this, &collider});
}

bool Bullet::UnitImpactDetector::operator()(CObject* object) const
{
    return bullet->hitsUnit(dynamic_cast<Unit*>(object), *collider);
}

bool Bullet::hitsUnit(Unit* unit, CObject& collider)
{
    if (!isAffectedUnit(unit, collider)) {
        return false;
    }

    damageUnit(unit);
    return true;
}

bool Bullet::isAffectedUnit(Unit* unit, CObject& collider)
{
    return unit != nullptr && isPlayerInvolved(unit) && collider.collideRect(unit);
}

bool Bullet::isPlayerInvolved(Unit* unit)
{
    /** if the player is shot down or the player did the shot */
    return unit->isPlayer() || this->shooter->isPlayer();
}

void Bullet::damageUnit(Unit* unit)
{
    unit->decreaseHealth();
    killUnitIfExhausted(unit);
}

void Bullet::killUnitIfExhausted(Unit* unit)
{
    if (unit->getHealth() == 0) {
        unit->setDead(true);
    }
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
