#ifndef COBJECT_H
#define COBJECT_H

#include <vector>
#include <SFML/Graphics.hpp>

class Window;
struct GameObjects;

class CObject {
public:
    CObject();
    CObject(int x, int y, int width, int height);
    CObject(GameObjects* go, int x, int y, int width, int height);
    virtual ~CObject();
    /**
     * sf::Drawable is required by Window 
     * to display simple SFML object on the screen
     * 
     * @return shape to draw 
     */
    virtual sf::Drawable* getDrawForm();
    
    /**
     * Draw the object with it's specific algorithm
     * 
     * @param window
     * @param dt Delayed time after last frame (in milliseconds / 1000)
     */
    virtual void draw(Window* window, float dt);
    
    /**
     * Check, is the point collide the current object (this)
     * 
     * @param x
     * @param y
     * 
     * @return bool
     */
    bool collidePoint(int x, int y);
    
    /**
     * Check, is obj is collided with current object (this)
     * 
     * @param obj
     * 
     * @return bool 
     */
    bool collideRect(CObject* obj);
    
    /**
     * Check, is the object collide background objects (all) after move
     * 
     * @param x
     * @param y
     * @return 
     */
    bool collideObjectAfterMove(float x, float y);
    
    /**
     * Get border points of the rectangle of the object
     * 
     * @return 
     */
    std::vector<std::vector<float>> getBorderPoints();
    
    //Getters
    float getX();
    float getY();
    int getWidth();
    int getHeight();

protected:
    GameObjects* go;
    float x;
    float y;
    int width;
    int height;
};

#endif /* COBJECT_H */