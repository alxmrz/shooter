#include <cmath>
#include <algorithm>
#include "AI.h"
#include "objects/units/Unit.h"
#include "objects/interactive/Platform.h"
#include "CObject.h"
#include "GameObjects.h"

namespace {
const float DETECTION_RADIUS = 300.f;
const float ENGAGEMENT_DISTANCE = 100.f;
const float SUPPORT_PROBE_DROP = 5.f;
const float LINE_OF_SIGHT_STEP = 10.f;

bool isSolidSightBlocker(Platform* platform, const sf::Vector2f& point) {
    return platform != nullptr && platform->isCollidable() && platform->collidePoint(point.x, point.y);
}

struct BlocksSight {
    sf::Vector2f point;

    bool operator()(CObject* object) const {
        return isSolidSightBlocker(dynamic_cast<Platform*>(object), point);
    }
};

bool sightBlockedByPlatforms(const std::vector<CObject*>& objects, const sf::Vector2f& point) {
    return std::any_of(objects.begin(), objects.end(), BlocksSight{point});
}

bool isSolidBelow(Platform* platform, CObject& probe) {
    return platform != nullptr && platform->isCollidable() && probe.collideRect(platform);
}

struct BlocksSupport {
    CObject* probe;

    bool operator()(CObject* object) const {
        return isSolidBelow(dynamic_cast<Platform*>(object), *probe);
    }
};

bool supportBelow(const std::vector<CObject*>& objects, CObject& probe) {
    return std::any_of(objects.begin(), objects.end(), BlocksSupport{&probe});
}
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
    aimAtPlayer();
    advanceOrFire(desiredDirection());
}

void AI::aimAtPlayer()
{
    sf::Vector2f target = playerCenter();
    unit->setAimTarget(target.x, target.y);
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
    return horizontalDistanceToPlayer() > ENGAGEMENT_DISTANCE && hasSupportAhead(direction);
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
    return isPlayerInRange() && hasLineOfSight();
}

bool AI::isPlayerInRange()
{
    return distanceToPlayer() <= DETECTION_RADIUS;
}

bool AI::hasLineOfSight()
{
    std::vector<sf::Vector2f> points = pathPoints();

    return std::none_of(points.begin(), points.end(), SightBlocker{this});
}

std::vector<sf::Vector2f> AI::pathPoints()
{
    const int steps = lineOfSightSteps();
    std::vector<sf::Vector2f> points;

    for (int i = 1; i < steps; i++) {
        points.push_back(pointOnPath(i, steps));
    }

    return points;
}

bool AI::SightBlocker::operator()(const sf::Vector2f& point) const
{
    return ai->isSightBlockedAt(point);
}

int AI::lineOfSightSteps()
{
    return static_cast<int>(distanceToPlayer() / LINE_OF_SIGHT_STEP);
}

sf::Vector2f AI::pointOnPath(int step, int steps)
{
    sf::Vector2f from = unitCenter();
    sf::Vector2f to = playerCenter();
    float t = static_cast<float>(step) / static_cast<float>(steps);

    return sf::Vector2f(from.x + (to.x - from.x) * t, from.y + (to.y - from.y) * t);
}

bool AI::isSightBlockedAt(sf::Vector2f point)
{
    return sightBlockedByPlatforms(go->all, point);
}

float AI::distanceToPlayer()
{
    sf::Vector2f from = unitCenter();
    sf::Vector2f to = playerCenter();
    float dx = to.x - from.x;
    float dy = to.y - from.y;

    return std::sqrt(dx * dx + dy * dy);
}

float AI::horizontalDistanceToPlayer()
{
    return std::fabs(go->player->getX() - unit->getX());
}

sf::Vector2f AI::unitCenter()
{
    return sf::Vector2f(
        unit->getX() + unit->getWidth() / 2.f,
        unit->getY() + unit->getHeight() / 2.f
    );
}

sf::Vector2f AI::playerCenter()
{
    return sf::Vector2f(
        go->player->getX() + go->player->getWidth() / 2.f,
        go->player->getY() + go->player->getHeight() / 2.f
    );
}

std::string AI::desiredDirection()
{
    return go->player->getX() < unit->getX() ? "left" : "right";
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

    return supportBelow(go->all, probe);
}

std::string AI::opposite(const std::string& direction)
{
    return direction == "left" ? "right" : "left";
}

void AI::setPatrolDirection(const std::string& direction)
{
    patrolDirection = direction;
}
