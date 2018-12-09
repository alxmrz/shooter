#ifndef SHOOTER_H
#define SHOOTER_H

#include <vector>
#include <map>
#include <string>
#include "../CObject.h"
#include "../GameObjects.h"

struct GameObjects;
class Window;

class Shooter: public CObject 
{
public:
    Shooter();
    Shooter(GameObjects* go, int x, int y, int width, int height);
    Shooter(const Shooter& orig);
    virtual ~Shooter();
    
    sf::Sprite* sprite;
    sf::Texture* texture;
    sf::Drawable* getDrawForm() override;
    
    void draw(Window* window, float dt) override;
    bool move(int x, int y);
    void setFormColor(sf::Color);
private:
    std::string direction = "right";
    int currentFrame = 0;
    float animationTime = 0.08;
    float elapsedTime = 0.0;
    bool isMoving = false;
    bool isJump = false;
    std::map<std::string, std::vector<std::vector<int>>> runSprites  {
        {
            "left", 
            {std::vector<int> {24,375,50,50},
            std::vector<int> {74,375,50,50},
            std::vector<int> {124,375,50,50},
            std::vector<int> {174,375,50,50},
            std::vector<int> {224,375,50,50},
            std::vector<int> {274,375,50,50},
            std::vector<int> {324,375,50,50},
            std::vector<int> {374,375,50,50}},
        },
        
        {
            "right", 
            {std::vector<int> {24,315,50,50},
            std::vector<int> {74,315,50,50},
            std::vector<int> {124,315,50,50},
            std::vector<int> {174,315,50,50},
            std::vector<int> {224,315,50,50},
            std::vector<int> {274,315,50,50},
            std::vector<int> {324,315,50,50},
            std::vector<int> {374,315,50,50}},
            
        },
    };
    
    std::map<std::string, std::vector<int>> jumpSprites {
        { "left", {124, 200, 50, 50}},
        {"right", {124, 144, 50, 50}}       
    };
    
    std::map<std::string, std::vector<int>> noMotionSprites {
        { "left", {24, 200, 50, 50}},
        {"right", {24, 144, 50, 50}}       
    };
};

#endif /* SHOOTER_H */

