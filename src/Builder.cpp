#include "Builder.h"
#include "CObject.h"
#include <SFML/Graphics.hpp>

Builder::Builder(){}

Builder::Builder(int x, int y, int width, int height): CObject(x, y, width, height)
{
    this->circle = sf::CircleShape(width/2);
    this->circle.setFillColor(sf::Color::Green);
    this->circle.setPosition(x+width/2, y+height/2);
}

sf::Drawable* Builder::getDrawForm()
{
    return &this->circle;
}

void Builder::move(int x, int y)
{
    this->circle.move(x, y);
}