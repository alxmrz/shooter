#include "Movable.h"
#include "../CObject.h"

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
    if (direction == "left") {
        if (velocity > -maxVelocity) {
            velocity -= acceleration;
        }
    } else if (direction == "right") {
        if (velocity < maxVelocity) {
            velocity += acceleration;
        }
    } else {
        velocity = 0.f;
    }
}

bool Movable::move(float x, float y)
{
    bool isMapEnd = this->x+x <= 0 || this->y + y <= 0;
    if (!isMapEnd && !collideObjectAfterMove(x, y) == true) {
        this->x += x;
        this->y += y;
        return true;
    } else if (y == 0.f) {
        velocity = 0;
    }
    
    return false;
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
    this->direction = direction;
}