#ifndef MOVABLE_H
#define MOVABLE_H

#include "../../CObject.h"

class Movable: public CObject {
public:
    Movable();
    Movable(GameObjects* go, float x, float y, int width, int height);
    Movable(const Movable& orig);
    virtual ~Movable();
    
    /**
     * Shift object position on x and y
     * 
     * @param x
     * @param y
     * @return is object moved
     */
    virtual bool move(float x, float y);
    virtual bool move(std::string direction); 
    virtual void stopMoving();
    bool isMoving();

    void calcVelocity();
    float getAcceleration();
    float getVelocity();
    float getMaxVelocity();

protected:
    float acceleration = 0.3f;
    /* Velocity when object is moving left or right*/
    float velocity = 0.f;
    /* Max object velocity while moving*/
    float maxVelocity = 5.f;
    /* is current object moving */
    bool moving = false;
    
    /* Direction of the object for sprite choose*/
    std::string direction = "right";
    void setDirection(std::string direction);
};

#endif /* MOVABLE_H */

