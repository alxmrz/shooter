#ifndef AI_H
#define AI_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class GameObjects;
class Unit;
class Platform;
class CObject;

class AI {
public:
    AI();
    AI(GameObjects* go, Unit* unit);
    AI(const AI& orig);
    virtual ~AI();
    virtual void think();

    // Public for testing: expose the perception and edge probes.
    bool detectsPlayer();
    bool hasSupportAhead(const std::string& direction);
    void setPatrolDirection(const std::string& direction);

private:
    GameObjects* go;
    Unit* unit;

    std::string patrolDirection = "right";

    bool isActive();
    void act();
    void engage();
    void aimAtPlayer();
    void advanceOrFire(const std::string& direction);
    void patrol();
    bool isPlayerInRange();
    bool hasLineOfSight();
    std::vector<sf::Vector2f> pathPoints();
    bool isSightBlockedAt(sf::Vector2f point);
    float distanceToPlayer();
    float horizontalDistanceToPlayer();
    sf::Vector2f unitCenter();
    sf::Vector2f playerCenter();
    int lineOfSightSteps();
    sf::Vector2f pointOnPath(int step, int steps);
    std::string desiredDirection();
    bool shouldApproach(const std::string& direction);
    float stepOffset(const std::string& direction);
    bool isSolidGroundBelow(float x, float y);
    static std::string opposite(const std::string& direction);

    struct SightBlocker {
        AI* ai;

        bool operator()(const sf::Vector2f& point) const;
    };
};

#endif /* AI_H */
