#ifndef SHOOTER_H
#define SHOOTER_H

#include <vector>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include "../CObject.h"
#include "../GameObjects.h"

struct GameObjects;
class Window;

class Shooter : public CObject {
public:
    Shooter();
    Shooter(GameObjects* go, float x, float y, int width, int height);
    Shooter(const Shooter& orig);
    virtual ~Shooter();
    
    /**
     * @var is current object moving
     */
    bool isMoving = false;
    
    /**
     * @var is current object jumping
     */
    bool isJump = false;
    bool isDead = false;
    bool isNeedToDie = false;
    bool isFalling = false;
    float acceleration = 0.3f;
    float velocity = 0.f;
    float velocityHorizontal = 0.f;
    float maxVelocity = 5.f;
    float currentJumpHeight = 0.f;
    
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
     * Shift object position on x and y
     * 
     * @param x
     * @param y
     * @return is moving done successfully
     */
    bool move(float x, float y);
    bool move();
    
    /**
     * Make a fire (it creates Bullet instance)
     */
    void fire();
    
    void jump();
    void runOperations();
    void setMainTexture(sf::Texture* texture);
    void setExplosionTexture(sf::Texture* texture);
    
private:
    /**
     * @var Sprite for drawing
     */
    sf::Sprite* sprite;
    
    /**
     * Texture for sprite (Texture loads image). Sprite contains the texture
     */
    sf::Texture* texture;
    sf::Texture* explosion;
    
    /**
     * Direction of the object for sprite choose
     */
    std::string direction = "right";
    
    /**
     * @var current frame for sprite choose
     */
    unsigned int currentFrame = 0;
    
    /**
     * Time to show new sprite
     */
    float animationTime = 0.08;
    
    /**
     * @var time passed after new sprite choose
     */
    float elapsedTime = 0.0;
    
    float fireTime = 0.f;

    /**
     * Sprites for showing run images depending on direction (left, right)
     */
    std::map<std::string, std::vector<std::vector<int>>> runSprites
    {
        {
            "left",
            {
                std::vector<int>{26, 375, 50, 50},
                std::vector<int>{76, 375, 50, 50},
                std::vector<int>{126, 375, 50, 50},
                std::vector<int>{176, 375, 50, 50},
                std::vector<int>{226, 375, 50, 50},
                std::vector<int>{276, 375, 50, 50},
                std::vector<int>{326, 375, 50, 50},
                std::vector<int>{376, 375, 50, 50}
            },
        },

        {
            "right",
            {
                std::vector<int>{26, 315, 50, 50},
                std::vector<int>{76, 315, 50, 50},
                std::vector<int>{126, 315, 50, 50},
                std::vector<int>{176, 315, 50, 50},
                std::vector<int>{226, 315, 50, 50},
                std::vector<int>{276, 315, 50, 50},
                std::vector<int>{326, 315, 50, 50},
                std::vector<int>{376, 315, 50, 50}
            },

        },
    };

    /**
     * @var sprite for jump action
     */
    std::map<std::string, std::vector<int>> jumpSprites
    {
        {
            "left",
            {
                124, 200, 50, 50
            }
        },
        {
            "right",
            {
                124, 144, 50, 50
            }
        }
    };

    /**
     * @var Sprites when object does not move
     */
    std::map<std::string, std::vector<int>> noMotionSprites
    {
        {
            "left",
            {
                24, 200, 50, 50
            }
        },
        {
            "right",
            {
                24, 144, 50, 50
            }
        }
    };
    
    std::vector<std::vector<int>> explosionSprites = {
        {2,0,80, 75},
        {80,0,80, 75},
        {160,0,80, 75},
        {240,0,80, 75},
        {320,0,75, 75},
        {400,0,0, 75},
    };
    
    std::multimap<std::string, std::vector<float>> operations;
};

#endif /* SHOOTER_H */

