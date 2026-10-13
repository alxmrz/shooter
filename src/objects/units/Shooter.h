#ifndef SHOOTER_H
#define SHOOTER_H

#include <vector>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../../GameObjects.h"
#include "../units/Unit.h"
#include "../interactive/ExplosionFrames.h"

struct GameObjects;
class Window;
class Text;
class Bullet;

class Shooter : public Unit {
public:
    Shooter(GameObjects* go, float x, float y, int width, int height);
    virtual ~Shooter();

    void destroy();

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
    void attack() override;
    
    void shiftSpritePositions() override;
    
    void setExplosionSprite(sf::Sprite* explosionSprite);
    void setCrystalCountText(Text* text);
    
    void onSecondJump() override;
    void setMainSprite(sf::Sprite* mainSprite);
    void applyRotation(float dt);
    void refreshSpriteFrame(float dt);
protected:
    /**
     * Create the bullet instance for a shot. Overridable for tests.
     */
    virtual Bullet* createBullet(float x, float y, int width, int height);
private:
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
    std::vector<sf::IntRect> explosionSprites = explosionFrameRects();
    
    bool isAirborne();
    void refreshGroundFrame();
    void refreshMovingFrame();
    void advanceRunFrame();
    void showRunFrame();
    void refreshJumpFrame();
    void refreshIdleFrame();
    void animateExplosion(float dt);
    void showNextExplosionFrame();
    void advanceExplosionFrame();
    void drawHearts(Window* window);
    void updateAnimation(float dt);
    void animateExplosionIfPending(float dt);
    void advanceTimers(float dt);
    bool isActiveAnimation();
    void drawHeartsIfNpc(Window* window);
    bool isAliveNpc();
    float spriteOriginX();
    float spriteOriginY();
    void rotateStep(float dt);
    float rotationSpeedPerSecond();
    void stopRotationIfComplete();
    void applySpriteRotationIfPresent();

    void fireBullet();
    bool canFire();
    void playShotgunSound();
    void performFire();
    void consumeAmmoIfPlayer();
    sf::Vector2f firingDirection();
    sf::Vector2f aimDirectionOrFallback();
    sf::Vector2f facingDirection();
    sf::Vector2f shooterCenter();
    sf::Vector2f bulletSpawn(sf::Vector2f direction);

    static constexpr float FIRE_INTERVAL = 0.5f;
    static constexpr float MIN_AIM_LENGTH = 0.001f;
    static constexpr float BULLET_SPAWN_OFFSET = 30.f;
    static constexpr int BULLET_WIDTH = 10;
    static constexpr int BULLET_HEIGHT = 10;
    
private:
    // Double jump rotation
    bool rotating = false;
    float rotationAngle = 0.f;
    void startRotation();
};

#endif /* SHOOTER_H */

