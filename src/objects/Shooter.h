#ifndef SHOOTER_H
#define SHOOTER_H

#include <vector>
#include <map>
#include <string>
#include "../CObject.h"
#include "../GameObjects.h"

struct GameObjects;
class Window;

class Shooter : public CObject {
public:
    Shooter();
    Shooter(GameObjects* go, int x, int y, int width, int height);
    Shooter(const Shooter& orig);
    virtual ~Shooter();
    

    
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
    bool move(int x, int y);
    
    /**
     * Make a fire (it creates Bullet instance)
     */
    void fire();
private:
    /**
     * @var Sprite for drawing
     */
    sf::Sprite* sprite;
    
    /**
     * Texture for sprite (Texture loads image). Sprite contains the texture
     */
    sf::Texture* texture;
    
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
    
    /**
     * @var is current object moving
     */
    bool isMoving = false;
    
    /**
     * @var is current object jumping
     */
    bool isJump = false;
    
    /**
     * Sprites for showing run images depending on direction (left, right)
     */
    std::map<std::string, std::vector<std::vector<int>>> runSprites
    {
        {
            "left",
            {
                std::vector<int>{24, 375, 50, 50},
                std::vector<int>{74, 375, 50, 50},
                std::vector<int>{124, 375, 50, 50},
                std::vector<int>{174, 375, 50, 50},
                std::vector<int>{224, 375, 50, 50},
                std::vector<int>{274, 375, 50, 50},
                std::vector<int>{324, 375, 50, 50},
                std::vector<int>{374, 375, 50, 50}
            },
        },

        {
            "right",
            {
                std::vector<int>{24, 315, 50, 50},
                std::vector<int>{74, 315, 50, 50},
                std::vector<int>{124, 315, 50, 50},
                std::vector<int>{174, 315, 50, 50},
                std::vector<int>{224, 315, 50, 50},
                std::vector<int>{274, 315, 50, 50},
                std::vector<int>{324, 315, 50, 50},
                std::vector<int>{374, 315, 50, 50}
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
};

#endif /* SHOOTER_H */

