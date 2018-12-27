#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../GameObjects.h"
#include "Movable.h"

struct GameObjects;
class Window;
class Text;

class Unit : public Movable {
public:
    Unit();
    Unit(GameObjects* go, float x, float y, int width, int height);
    Unit(const Unit& orig);
    virtual ~Unit();
    
    /**
     * Make a fire (it creates Bullet instance)
     */
    virtual void attack();
    virtual void think();
    
    bool isPlayer();
    bool isDead();
    /* need to remove the current object? */
    bool remove();

    using Movable::move;
    
    bool move(float x, float y) override;
    
    virtual void shiftSpritePositions();
    
    /**
     * Shift coordinates on the Y scale
     */
    virtual void jump();
    
    /**
     *  Shift coordinates on the Y scale when gravitation is called
     *  Used by GameState::causeGravity()
     */
    virtual void gravitate();
    
    virtual void stopJumping();

    float getCurrentJumpHeight();
    float getVelocityHorizontal();

    bool isFalling();
    bool isJumping();
    int getHealth();
    int getCrystals();
    sf::Drawable* getDrawForm() override;
    void decreaseHealth();
    
    void setMain(bool main);
    void setDead(bool dead);

   
    void setMainSprite(sf::Sprite* mainSprite);
    void setHeartSprite(sf::Sprite* heartSprite);
    void setJumpSound(sf::Sound* jumpSound);
    void setCrystalSound(sf::Sound* jumpSound);
    void setShotgunSound(sf::Sound* jumpSound);
protected:
    /* is the current object dead*/
    bool dead = false;
    /* must the current object be deleted */
    bool mustBeDeleted = false;

    /* is current object controlled by Player*/
    bool main = false;
    int health = 3;
    int crystals = 0;

    /* When object jump, it can not jump higher than the value of the variable */
    float currentJumpHeight = 0.f;
    /* Velocity when jump*/
    float velocityHorizontal = 0.f;

    float gravityPower = 0.1f;    
    float gravitationalVelocity = 0.f;
    float gravitationalAcceleration = 0.5f;
    /* Time for gravitational formula*/
    float gravitationalTime = 0.0;

    /* @var is current object jumping */
    bool jumping = false;
 
    /* is current object falling */
    bool falling = false;
    
    sf::Sound* jumpSound;
    sf::Sound* shotgunSound;
    sf::Sound* crystalSound;

    sf::Sprite* mainSprite;
    sf::Sprite* heartSprite;
    sf::Sprite* explosionSprite;
    

    
    /*  @var current frame for sprite choose */
    unsigned int currentFrame = 0;
    
    /* Time to show new sprite */
    float animationTime = 0.08;
    
    /* time passed after new sprite choose */
    float elapsedTime = 0.0;

    /* Time after last fire call */
    float fireTime = 0.f; 
    
    bool isMoveRight = true;
    bool isMoveLeft = false;
    
    /**
     * If a crystall collide it will be collected and deleted
     * 
     * @return is crystal collected
     */
    bool collectCollidedCrystal();
    
    bool isNextFalling(std::string direction);
    void animateMoving();
};

#endif /* UNIT_H */

