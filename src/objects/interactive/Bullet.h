#ifndef BULLET_H
#define BULLET_H

#include <string>
#include <SFML/Audio.hpp>
#include "../../CObject.h"

namespace sf {
    class Drawable;
}
class GameObjects;
class Unit;

class Bullet : public CObject {
public:
    Bullet();
    Bullet(Unit* shooter, GameObjects* go, int x, int y, int width, int height);
    Bullet(const Bullet& orig);
    virtual ~Bullet();
    
    /**
     * Shift object position on x and y
     * 
     * @param x
     * @param y
     * @return is moving done successfully
     */
    bool move(float x, float y);
    
    /**
     * Set direction the objects will be moving
     * 
     * @param direction
     */
    void setDirection(std::string direction);
    
    sf::Sound* explosionSound;
    void setExplosionSound(sf::Sound* sound);
private:
    /** Shooter, that did the bullet */
    Unit* shooter;
    
    /** Direction of the object to move */
    std::string direction;
    
    /** When the object will be at this position the object will be deleted */
    std::vector<int> endPosition;

    /**
     * Check, is current object collide Playable object when moving
     * 
     * @param x
     * @param y
     * @return bool
     */
    bool collidePlayableAfterMove(float x, float y);
};

#endif /* BULLET_H */

