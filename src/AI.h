#ifndef AI_H
#define AI_H

#include <string>

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
    void advanceOrFire(const std::string& direction);
    void patrol();
    bool isPlayerOnY();
    bool isPlayerOnX();
    std::string desiredDirection();
    float distanceToPlayer();
    bool shouldApproach(const std::string& direction);
    float stepOffset(const std::string& direction);
    bool isSolidGroundBelow(float x, float y);
    bool isSolidBelow(Platform* platform, CObject& probe);
    static std::string opposite(const std::string& direction);
};

#endif /* AI_H */
