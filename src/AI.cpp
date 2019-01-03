#include "AI.h"
#include "objects/units/Unit.h"
#include "GameObjects.h"

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
    if (!unit->isPlayer() && !unit->isDead()) { 
        
        if (isPlayerOnY() && isPlayerOnXLeft()) {
            isMoveRight = false;
            isMoveLeft = true;
        } else if (isPlayerOnY() && isPlayerOnXRight()) {
           isMoveRight = true;
           isMoveLeft = false; 
        }
           
        bool isPlayerOnFireLine = (isPlayerOnXRight() || isPlayerOnXLeft()) && isPlayerOnY();
        
        if (isMoveRight) {
            isMoveRight = move("right");
            
            if (isPlayerOnFireLine) {
                unit->attack();
            }
            
            isMoveLeft = !isMoveRight;
        } else if (isMoveLeft) {
            isMoveLeft = move("left");
            
            if (isPlayerOnFireLine) {
                unit->attack();
            }
            isMoveRight = !isMoveLeft;
        }
    }
}

bool AI::move(std::string direction)
{
    return unit->move(direction) && !isNextFalling(direction);
}

bool AI::isNextFalling(std::string direction)
{
    int offsetX = 0;
    if (direction == "left") {
        offsetX = -25;
    } else if (direction == "right") {
        offsetX = 25;
    }
    Unit fake = Unit(this->go, unit->getX()+offsetX, unit->getY(), unit->getWidth(), unit->getHeight());
    if (!fake.collideObjectAfterMove(0.f, 5.f)) {
        unit->setVelocity(0);
        return true;
    } 
    return false;
}

bool AI::isPlayerOnY()
{
    return unit->getY() - 25 <= go->player->getY() && 
        go->player->getY() <= unit->getY() + 25;
}

bool AI::isPlayerOnXLeft()
{
    return unit->getX() - 300 <= go->player->getX() && go->player->getX() <= unit->getX();
}

bool AI::isPlayerOnXRight()
{
    return unit->getX()<= go->player->getX() && go->player->getX() <= unit->getX() + 300;;
}