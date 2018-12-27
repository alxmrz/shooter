#ifndef SHOOTER_H
#define SHOOTER_H

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
     * sf::Drawable is required by Window 
     * to display simple SFML object on the screen
     * 
     * @return shape to draw 
     */
    sf::Drawable* getDrawForm() override;

    /**
     * Draw the object with it's specific algorithm
     * 
     * @param window
     * @param dt Delayed time after last frame (in milliseconds / 1000)
     */
    void draw(Window* window, float dt) override;
    
    /**
     * Make a fire (it creates Bullet instance)
     */
    void fire();
    void think();
    
    bool isPlayer();
    bool isDead();
    /* need to remove the current object? */
    bool remove();

    using Movable::move;
    
    bool move(float x, float y) override;
    
    void shiftSpritePositions();
    
    /**
     * Shift coordinates on the Y scale
     */
    void jump();
    
    /**
     *  Shift coordinates on the Y scale when gravitation is called
     *  Used by GameState::causeGravity()
     */
    void gravitate();
    
    void stopJumping();

    float getCurrentJumpHeight();
    float getVelocityHorizontal();

    bool isFalling();
    bool isJumping();
    int getHealth();
    int getCrystals();
    
    void decreaseHealth();
    
    void setMain(bool main);
    void setDead(bool dead);


    void setMainSprite(sf::Sprite* mainSprite);
    void setHeartSprite(sf::Sprite* heartSprite);
    void setExplosionSprite(sf::Sprite* explosionSprite);
    void setJumpSound(sf::Sound* jumpSound);
    void setCrystalSound(sf::Sound* jumpSound);
    void setShotgunSound(sf::Sound* jumpSound);
    void setCrystalCountText(Text* text);
private:
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

    sf::Sprite* shooterSprite;
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

    /* Sprites map for showing run images depending on direction (left, right) */
    std::map<std::string, std::vector<sf::IntRect>> runSprites
    {
        {
            "left",
            {
                sf::IntRect(26, 375, 50, 50),
                sf::IntRect(76, 375, 50, 50),
                sf::IntRect(126, 375, 50, 50),
                sf::IntRect(176, 375, 50, 50),
                sf::IntRect(226, 375, 50, 50),
                sf::IntRect(276, 375, 50, 50),
                sf::IntRect(326, 375, 50, 50),
                sf::IntRect(376, 375, 50, 50)
            },
        },

        {
            "right",
            {
                sf::IntRect(26, 315, 50, 50),
                sf::IntRect(76, 315, 50, 50),
                sf::IntRect(126, 315, 50, 50),
                sf::IntRect(176, 315, 50, 50),
                sf::IntRect(226, 315, 50, 50),
                sf::IntRect(276, 315, 50, 50),
                sf::IntRect(326, 315, 50, 50),
                sf::IntRect(376, 315, 50, 50)
            },

        },
    };

    /* sprite map for jump action */
    std::map<std::string, sf::IntRect> jumpSprites
    {
        {"left",sf::IntRect(124, 200, 50, 50)},
        {"right", sf::IntRect(124, 144, 50, 50)}
    };

    /* Sprites map when object does not move */
    std::map<std::string, sf::IntRect> noMotionSprites
    {
        {"left", sf::IntRect(24, 200, 50, 50) },
        {"right",sf::IntRect(24, 144, 50, 50) }
    };
    
    /* Sprites map for explosion animation */
    std::vector<sf::IntRect> explosionSprites 
    {
        sf::IntRect(2,0,80, 75),
        sf::IntRect(80,0,80, 75),
        sf::IntRect(160,0,80, 75),
        sf::IntRect(240,0,80, 75),
        sf::IntRect(320,0,75, 75),
        sf::IntRect(400,0,0, 75),
    };
    
    bool isMoveRight = true;
    bool isMoveLeft = false;
    
    /**
     * If a crystall collide it will be collected and deleted
     * 
     * @return is crystal collected
     */
    bool collectCollidedCrystal();
    
    bool isNextFalling(std::string direction);
    void animateRun();
    void animateExplosion();
    void drawHearts(Window* window);
};

#endif /* SHOOTER_H */

