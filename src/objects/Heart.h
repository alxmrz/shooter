#ifndef HEART_H
#define HEART_H

#include "../CObject.h"

class Heart: public CObject {
public:
    Heart();
    Heart(const Heart& orig);
    Heart(sf::Texture* texture, int x, int y, int width, int height);
    virtual ~Heart();
    
    /**
     * @var Sprite for drawing
     */
    sf::Sprite* sprite;
    
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
    void move(int x, int y);
private:

};

#endif /* HEART_H */

