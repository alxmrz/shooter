#include <SFML/Graphics.hpp>
#include "Button.h"
#include "../CObject.h"

Button::Button() {
}

Button::Button(int x, int y, int width, int height): CObject(x, y, width, height)
{
    shape = new sf::RectangleShape(sf::Vector2f(width, height));
    shape->setOutlineColor(sf::Color::Red);
    shape->setOutlineThickness(5);
    shape->setPosition(x, y);
}

Button::~Button() 
{
}

sf::Drawable* Button::getDrawForm()
{
    return shape;
}
