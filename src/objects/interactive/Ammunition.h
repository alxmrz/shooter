#ifndef AMMUNITION_H
#define AMMUNITION_H

#include <SFML/Graphics.hpp>
#include "../../CObject.h"

class Ammunition: public CObject {
public:
    Ammunition();
    Ammunition(const Ammunition& orig);
    Ammunition(sf::Texture* texture, int x, int y, int width, int height);
    virtual ~Ammunition();
    
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

#endif /* AMMUNITION_H */

