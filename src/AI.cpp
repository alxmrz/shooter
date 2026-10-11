#include <cmath>
#include "AI.h"
#include "objects/units/Unit.h"
#include "objects/interactive/Platform.h"
#include "CObject.h"
#include "GameObjects.h"

namespace {
const int DETECTION_RANGE_X = 300;
const int DETECTION_RANGE_Y = 25;
const float ENGAGEMENT_DISTANCE = 100.f;
const float SUPPORT_PROBE_DROP = 5.f;
}

AI::AI() {
}

AI::AI(GameObjects* go, Unit* unit): go(go), unit(unit) {
}

AI::AI(const AI& orig) {
}

AI::~AI() {
}

void AI::think()
{
    if (isActive()) {
        act();
    }
}

bool AI::isActive()
{
    return !unit->isPlayer() && !unit->isDead();
}

void AI::act()
{
    if (detectsPlayer()) {
        engage();
        return;
    }

    patrol();
}

void AI::engage()
{
    advanceOrFire(desiredDirection());
}

void AI::advanceOrFire(const std::string& direction)
{
    unit->setDirection(direction);

    if (shouldApproach(direction)) {
        unit->move(direction);
        return;
    }

    unit->attack();
}

bool AI::shouldApproach(const std::string& direction)
{
    return distanceToPlayer() > ENGAGEMENT_DISTANCE && hasSupportAhead(direction);
}

void AI::patrol()
{
    if (!hasSupportAhead(patrolDirection)) {
        patrolDirection = opposite(patrolDirection);
    }

    unit->move(patrolDirection);
}

bool AI::detectsPlayer()
{
    return isPlayerOnX() && isPlayerOnY();
}

bool AI::isPlayerOnX()
{
    return std::fabs(go->player->getX() - unit->getX()) <= DETECTION_RANGE_X;
}

bool AI::isPlayerOnY()
{
    return std::fabs(go->player->getY() - unit->getY()) <= DETECTION_RANGE_Y;
}

std::string AI::desiredDirection()
{
    return go->player->getX() < unit->getX() ? "left" : "right";
}

float AI::distanceToPlayer()
{
    return std::fabs(go->player->getX() - unit->getX());
}

bool AI::hasSupportAhead(const std::string& direction)
{
    return isSolidGroundBelow(unit->getX() + stepOffset(direction), unit->getY());
}

float AI::stepOffset(const std::string& direction)
{
    return direction == "left"
        ? -static_cast<float>(unit->getWidth())
        : static_cast<float>(unit->getWidth());
}

bool AI::isSolidGroundBelow(float x, float y)
{
    CObject probe(x, y + SUPPORT_PROBE_DROP, unit->getWidth(), unit->getHeight());

    for (CObject* object : go->all) {
        Platform* platform = dynamic_cast<Platform*>(object);
        if (isSolidBelow(platform, probe)) {
            return true;
        }
    }

    return false;
}

bool AI::isSolidBelow(Platform* platform, CObject& probe)
{
    return platform != nullptr && platform->isCollidable() && probe.collideRect(platform);
}

std::string AI::opposite(const std::string& direction)
{
    return direction == "left" ? "right" : "left";
}

void AI::setPatrolDirection(const std::string& direction)
{
    patrolDirection = direction;
}
