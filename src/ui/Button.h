#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <SFML/Graphics.hpp>
#include "../CObject.h"
#include "Text.h"

class Text;
class Window;

class Button: public CObject 
{
public:
    Button();
    Button(const CObject& orig);
    Button(std::string id, std::string text, int x, int y, int width, int height);
    virtual ~Button();
    void draw(Window* window) override;
    sf::Drawable* getDrawForm() override;
    std::string getId();
    void setBorderColor(sf::Color);
    void clicked();
    void hovered();
    void defaultState();
    
    
private:
    sf::RectangleShape* shape;
    Text* text;
    std::string id;
};

#endif /* BUTTON_H */

