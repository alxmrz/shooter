#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "../CObject.h"
#include "Text.h"

Button::Button() {
}

Button::Button(std::string text, int x, int y, int width, int height): CObject(x, y, width, height)
{
    this->text = new Text(text, x, y);
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

void Button::draw(Window* window)
{
    //std::cout<< "Button draw";
    //window->draw(*shape);
    window->draw(*text->getDrawForm());
}
