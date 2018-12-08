#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H


#include "CObject.h"
#include "objects/Shooter.h"


class Shooter;
class CObject;

struct GameObjects 
{
    GameObjects();
    
    std::vector<CObject*> buttons;
    std::vector<CObject*> background;
    std::vector<CObject*> playable;
    Shooter* player;
    
    virtual ~GameObjects();
    std::vector<CObject*>* all();
    void reset();
};


#endif /* GAMEOBJECTS_H */

