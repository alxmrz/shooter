#ifndef GROUND_H
#define GROUND_H

#include "../CObject.h"

class Ground : public CObject {
public:
    Ground();
    Ground(const Ground& orig);
    Ground(int x, int y, int width, int height);
    virtual ~Ground();

    /**
     * @var shape Shape to draw in Window
     */
    sf::RectangleShape* shape;
    
    /**
     * @var Sprite for drawing
     */
    sf::Sprite* sprite;
    
    /**
     * Texture for sprite (Texture loads image). Sprite contains the texture
     */
    sf::Texture* texture;
    
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

#endif /* GROUND_H */

