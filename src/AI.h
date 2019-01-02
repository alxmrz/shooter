#ifndef AI_H
#define AI_H

#include <string>

class GameObjects;
class Unit;

class AI {
public:
    AI();
    AI(GameObjects* go, Unit* unit);
    AI(const AI& orig);
    virtual ~AI();
    virtual void think();
private:
    GameObjects* go;
    Unit* unit;
    
    bool isMoveRight = true;
    bool isMoveLeft = false;
    
    bool isNextFalling(std::string direction);
    bool isPlayerOnY();
    bool isPlayerOnXLeft();
    bool isPlayerOnXRight();
};

#endif /* AI_H */

