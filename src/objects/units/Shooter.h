#ifndef SHOOTER_H
#define SHOOTER_H

#include <vector>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../../GameObjects.h"
#include "../units/Unit.h"

struct GameObjects;
class Window;
class Text;

class Shooter : public Unit {
public:
    Shooter();
    Shooter(GameObjects* go, float x, float y, int width, int height);
    Shooter(const Shooter& orig);
    virtual ~Shooter();

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
    std::vector<sf::IntRect> explosionSprites 
    {
        sf::IntRect(2,0,80, 75),
        sf::IntRect(80,0,80, 75),
        sf::IntRect(160,0,80, 75),
        sf::IntRect(240,0,80, 75),
        sf::IntRect(320,0,75, 75),
        sf::IntRect(400,0,0, 75),
    };
    
    void animateRun();
    void animateExplosion();
    void drawHearts(Window* window);
};

#endif /* SHOOTER_H */

