#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <SFML/Graphics.hpp>
#include "../CObject.h"
#include "Text.h"

class Text;
class Window;

class Button : public CObject {
public:
    Button();
    Button(const CObject& orig);
    Button(std::string id, std::string text, int x, int y, int width, int height);
    virtual ~Button();
    
    /**
     * Draw the object with it's specific algorithm
     * 
     * @param window
     * @param dt Delayed time after last frame (in milliseconds / 1000)
     */
    void draw(Window* window, float dt) override;
    
    /**
     * sf::Drawable is required by Window 
     * to display simple SFML object on the screen
     * 
     * @return shape to draw 
     */
    sf::Drawable* getDrawForm() override;
    
    /**
     * Get id of the button.
     * 
     * @return 
     */
    std::string getId();
    
    /**
     * Set border color of the current button
     * 
     * @param sf::Color object for representing RGB color (sf::Color(255,255,255))
     */
    void setBorderColor(sf::Color);
    
    /**
     * React for click event
     */
    void clicked();
    
    /**
     * React for hover event
     */
    void hovered();
    
    /**
     * Make default state of the button
     */
    void defaultState();


private:
    /**
     * @var shape Shape to draw in Window
     */
    sf::RectangleShape* shape;
    
    /**
     * @var Text of the button
     */
    Text* text;
    
    /**
     * @var id of the button. 
     * Need to check unique id when creating instances
     */
    std::string id;
};

#endif /* BUTTON_H */

