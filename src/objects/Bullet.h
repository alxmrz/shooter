#ifndef BULLET_H
#define BULLET_H

#include <string>
#include "../CObject.h"

namespace sf {
    class Drawable;
}
class GameObjects;


class Bullet : public CObject {
public:
    Bullet();
    Bullet(GameObjects* go, int x, int y, int width, int height);
    Bullet(const Bullet& orig);
    virtual ~Bullet();

    /**
     * sf::Drawable is required by Window 
     * to display simple SFML object on the screen
     * 
     * @return shape to draw 
     */
    sf::Drawable* getDrawForm() override;
    
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
     * @var direction Direction of the object to move 
     */
    std::string direction;
    
    /**
     * @var endPosition When the object will be at this position the object will be deleted
     */
    std::vector<int> endPosition;

    /**
     * Check, is current object collide Playable object when moving
     * 
     * @param x
     * @param y
     * @return bool
     */
    bool collidePlayableAfterMove(int x, int y);
};

#endif /* BULLET_H */

