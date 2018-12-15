#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "../CObject.h"
#include "../Window.h"
#include "Text.h"

Button::Button()
{
}

Button::Button(const CObject& orig)
{
}

Button::Button(int x, int y, int width, int height)
: CObject(x, y, width, height)
{
    shape = new sf::RectangleShape(sf::Vector2f(width, height));
    shape->setOutlineColor(sf::Color::Red);
    shape->setOutlineThickness(5);
    shape->setPosition(x, y);
}

Button::~Button()
{
}

void Button::setBorderColor(sf::Color color)
{
    shape->setOutlineColor(color);
}


sf::Drawable* Button::getDrawForm()
{
    return shape;
}

void Button::draw(Window* window, float dt)
{
    window->draw(*shape);
    window->draw(*text->getDrawForm());
}

std::string Button::getId()
{
    return id;
}

void Button::setId(std::string id)
{
    this->id = id;
}
void Button::setText(Text* text)
{
    this->text = text;
}

void Button::clicked()
{
    shape->setOutlineColor(sf::Color::Green);
    text->message->setFillColor(sf::Color::Green);
}

void Button::hovered()
{
    shape->setOutlineColor(sf::Color::Blue);
    text->message->setFillColor(sf::Color::Black);
}

void Button::defaultState()
{
    shape->setOutlineColor(sf::Color::Red);
    text->message->setFillColor(sf::Color::Black);
}