#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../../GameObjects.h"
#include "../interfaces/Movable.h"

#include "../../Event.h"

struct GameObjects;
class Window;
class Text;
class AI;
class Platform;

class Unit : public Movable {
public:
    Unit(GameObjects* go, float x, float y, int width, int height);
    virtual ~Unit();
    
    /**
     * Make a fire (it creates Bullet instance)
     */
    virtual void attack();
    
    bool isPlayer();
    bool isDead();
    /* need to remove the current object? */
    bool remove();
    /* check if object marked for deletion */
    bool isMustBeDeleted();

    using Movable::move;
    using Movable::setDirection;
    
    void update(Event event);
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
    
    /**
     * Called when performing second jump in air (for special effects)
     */
    virtual void onSecondJump();

    float getCurrentJumpHeight();
    float getVelocityHorizontal();

    bool isFalling();
    bool isJumping();
    bool canJump();
    int getHealth();
    int getAmmo();
    int getCrystals();
    sf::Drawable* getDrawForm() override;
    void decreaseHealth();
    
    void setMain(bool main);
    void setDead(bool dead);
    
    void think();
    void setMainSprite(sf::Sprite* mainSprite);
    void setHeartSprite(sf::Sprite* heartSprite);
    void setJumpSound(sf::Sound* jumpSound);
    void setCrystalSound(sf::Sound* jumpSound);
    void setShotgunSound(sf::Sound* jumpSound);
    void setVelocity(float value);
protected:
    AI* ai;
    
    /* is the current object dead*/
    bool dead = false;
    /* must the current object be deleted */
    bool mustBeDeleted = false;
    bool isPlatformAffected = false;
    
    /* is current object controlled by Player*/
    bool main = false;
    int health = 3;
    int ammo = 10;
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
  
    /* Jump system */
    int jumpCount = 0;
    const int maxJumps = 2;
    bool jumpCooldown = false;
    int jumpCooldownFrames = 0;
    bool isGrounded();
    void resetJumpCount();
    
    void animateMoving();

private:
    void collectItems();
    bool resolvePlatformCollision(float x, float y);
    Platform* findCollidingPlatform();
    bool isSolidCollision(Platform* platform);
    bool applyPlatformCollision(Platform* platform, float x, float y);
    bool applyVerticalCollision(Platform* platform, float x, float y);
    bool landOnPlatform(Platform* platform);
    bool blockMoveUp(float x, float y);
    bool blockMoveHorizontal(float x, float y);
    void revertMove(float x, float y);
};

#endif /* UNIT_H */

