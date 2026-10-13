#include "Movable.h"
#include "../../CObject.h"

Movable::Movable() {
}

Movable::Movable(GameObjects* go, float x, float y, int width, int height) 
: CObject(go, x, y, width, height)
{
    
}

Movable::Movable(const Movable& orig) {
}

Movable::~Movable() {
}


void Movable::stopMoving()
{
    moving = false;
    velocity = 0.f;
}

bool Movable::isMoving()
{
    return moving;
}

void Movable::calcVelocity()
{
    if (isDirection("left")) {
        decelerateLeft();
        return;
    }

    applyDirectionVelocity();
}

bool Movable::isDirection(const std::string& value) const
{
    return direction == value;
}

void Movable::decelerateLeft()
{
    if (velocity > -maxVelocity) {
        velocity -= acceleration;
    }
}

void Movable::accelerateRight()
{
    if (velocity < maxVelocity) {
        velocity += acceleration;
    }
}

void Movable::applyDirectionVelocity()
{
    if (isDirection("right")) {
        accelerateRight();
        return;
    }

    velocity = 0.f;
}

bool Movable::move(float x, float y)
{
    if (canMoveTo(x, y)) {
        applyMove(x, y);
        return true;
    }

    resetVelocityAtGround(y);
    return false;
}

bool Movable::canMoveTo(float x, float y)
{
    return !isMapEnd(x, y) && !collideObjectAfterMove(x, y);
}

bool Movable::isMapEnd(float x, float y)
{
    return this->x + x <= 0 || this->y + y <= 0;
}

void Movable::applyMove(float x, float y)
{
    this->x += x;
    this->y += y;
}

void Movable::resetVelocityAtGround(float y)
{
    if (y == 0.f) {
        velocity = 0;
    }
}

bool Movable::move(std::string direction) 
{
    setDirection(direction);
    calcVelocity();
    moving = move(velocity, 0.f);
    
    return moving;
}

float Movable::getAcceleration()
{
    return acceleration;
}

float Movable::getVelocity()
{
    return velocity;
}

float Movable::getMaxVelocity()
{
    return maxVelocity;
}

void Movable::setDirection(std::string direction)
{
    if (this->direction != direction) {
        // Direction changed, reset velocity for instant response
        velocity = 0.f;
    }
    this->direction = direction;
}