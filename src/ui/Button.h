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
    Button(std::string text, int x, int y, int width, int height);
    virtual ~Button();
    void draw(Window* window) override;
    sf::Drawable* getDrawForm() override;
private:
    sf::RectangleShape* shape;
    Text* text;
};

#endif /* BUTTON_H */

