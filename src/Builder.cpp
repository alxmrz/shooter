#include "CObject.h"
#include "Builder.h"

#include <SFML/Graphics.hpp>

Builder::Builder () { }

Builder::Builder (int x, int y, int width, int height) : CObject (x, y, width, height)
{
  this->circle = new sf::CircleShape (width / 2);
  this->circle->setFillColor (sf::Color::Green);
  this->circle->setPosition (x, y);
}

sf::Drawable*
Builder::getDrawForm ()
{
  return this->circle;
}

void
Builder::move (int x, int y)
{
  this->x += x;
  this->y += y;
  this->circle->move (x, y);
}

void
Builder::setFormColor (sf::Color color)
{
  this->circle->setFillColor (color);
}