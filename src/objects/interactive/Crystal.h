#ifndef CRYSTAL_H
#define CRYSTAL_H

#include "../../CObject.h"

class Crystal: public CObject {
public:
    Crystal();
    Crystal(const Crystal& orig);
    Crystal(sf::Texture* texture, int x, int y, int width, int height);
    virtual ~Crystal();
    
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

#endif /* CRYSTAL_H */

