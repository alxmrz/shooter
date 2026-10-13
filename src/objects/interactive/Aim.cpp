#include <cmath>
#include "Aim.h"
#include "../../GameObjects.h"
#include "../../Window.h"

namespace {
const float MIN_DIRECTION_LENGTH = 0.001f;
}

Aim::Aim(GameObjects* go, float x, float y, int width, int height)
: CObject(go, x, y, width, height), target(x, y)
{
}

Aim::~Aim()
{
}

void Aim::setTarget(float x, float y)
{
    target.x = x;
    target.y = y;
    this->x = x;
    this->y = y;
    positionSprite();
}

void Aim::positionSprite()
{
    if (mainSprite == nullptr) {
        return;
    }

    mainSprite->setPosition(target.x, target.y);
}

float Aim::getTargetX()
{
    return target.x;
}

float Aim::getTargetY()
{
    return target.y;
}

sf::Vector2f Aim::getDirectionFrom(float fromX, float fromY)
{
    sf::Vector2f delta(target.x - fromX, target.y - fromY);
    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    if (length < MIN_DIRECTION_LENGTH) {
        return sf::Vector2f(0.f, 0.f);
    }

    return sf::Vector2f(delta.x / length, delta.y / length);
}

void Aim::draw(Window* window, float dt)
{
    if (isHidden()) {
        return;
    }

    CObject::draw(window, dt);
}

bool Aim::isHidden()
{
    return go == nullptr || go->gameOver;
}
