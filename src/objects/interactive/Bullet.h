#ifndef BULLET_H
#define BULLET_H

#include <string>
#include <vector>
#include <SFML/Audio.hpp>
#include "../../CObject.h"
#include "ExplosionFrames.h"

#include "../../Event.h"


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
    void update(Event event) override;
    /**
     * Draw the object with it's specific algorithm
     * 
     * @param window
     * @param dt Delayed time after last frame (in milliseconds / 1000)
     */
    void draw(Window* window, float dt) override;
    /**
     * Set direction the objects will be moving
     * 
     * @param direction
     */
    void setDirection(std::string direction);

    /**
     * Set the (unit) direction the bullet flies along and rotate its sprite.
     */
    void setFlightDirection(sf::Vector2f direction);

    float getVelocityX();
    float getVelocityY();
    float getRotation();
    
    sf::Sound* explosionSound = nullptr;
    void setExplosionSound(sf::Sound* sound);
    void setExplosionSprite(sf::Sprite* explosionSprite);
    
    bool isExploding();
    unsigned getExplosionFrame();
    int getExplosionSoundPlays();
    sf::Sprite* getExplosionSprite();
private:
    /** Shooter, that did the bullet */
    Unit* shooter;
    
    /** Unit direction (unit vector) the bullet flies along */
    sf::Vector2f flightDir = sf::Vector2f(1.f, 0.f);

    /** Position where the bullet was spawned */
    sf::Vector2f startPosition;

    /** Maximum flight distance from the spawn position */
    static constexpr float MAX_DISTANCE = 500.f;

    /** Movement step per simulation frame (20% less than the original 10) */
    static constexpr float BULLET_SPEED = 8.f;

    /** Number of updates between two explosion animation frames */
    static constexpr int EXPLOSION_FRAME_DELAY = 5;

    /** Whether the bullet reached its target and plays the explosion */
    bool exploding = false;

    /** Current frame of the explosion animation */
    unsigned explosionFrame = 0;

    /** Updates elapsed since the current explosion frame was shown */
    int explosionTick = 0;

    /** How many times the explosion sound was requested to play */
    int explosionSoundPlays = 0;

    /** Sprite used to render the explosion animation */
    sf::Sprite* explosionSprite = nullptr;

    /** Sprites map for explosion animation */
    std::vector<sf::IntRect> explosionFrames = explosionFrameRects();

    /**
     * Check, is current object collide Playable object when moving
     * 
     * @param x
     * @param y
     * @return bool
     */
    bool collidePlayableAfterMove(float x, float y);
    bool blockedAt(float x, float y);
    bool collidesAt(float x, float y);
    void advance(float x, float y);
    bool hitsUnit(Unit* unit, CObject& collider);
    bool isAffectedUnit(Unit* unit, CObject& collider);
    bool isPlayerInvolved(Unit* unit);
    void damageUnit(Unit* unit);
    void killUnitIfExhausted(Unit* unit);

    struct UnitImpactDetector {
        Bullet* bullet;
        CObject* collider;

        bool operator()(CObject* object) const;
    };

    /**
     * Check, is the next position beyond the maximum flight distance
     * 
     * @param nextX
     * @param nextY
     * @return bool
     */
    bool isMaxDistanceReached(float nextX, float nextY);

    /** Keep the sprite centered on the bullet collider */
    void positionSprite();

    /** Rotate the sprite to match the current flight direction */
    void applySpriteRotation();

    /** Return a unit-length copy of the given direction or the default one */
    sf::Vector2f normalizedDirection(sf::Vector2f direction);

    /** Advance the explosion animation while it is playing */
    void updateExplosion();

    /** Check, is a new explosion frame due */
    bool shouldAdvanceExplosion();

    /** Switch to the next explosion frame or remove the bullet when done */
    void advanceExplosion();

    /** Apply the current explosion frame to the sprite */
    void applyExplosionFrame();

    /** Move the bullet while it is flying */
    void updateFlight();

    /** Begin the explosion: stop moving, play sound and show the first frame */
    void startExplosion();

    /** Play the explosion sound at most once */
    void playExplosionSound();

    /** Place the explosion sprite at the bullet position */
    void positionExplosionSprite();
};

#endif /* BULLET_H */
