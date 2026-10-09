#include <SFML/Graphics.hpp>
#include "Platform.h"

Platform::Platform() {
}

Platform::Platform(const Platform& orig) {
}

Platform::Platform(int x, int y, int width, int height) : CObject(x, y, width, height)
{
    rect = new sf::RectangleShape;
    rect->setSize(sf::Vector2f(width, height));
    rect->setOutlineColor(sf::Color::Black);
    rect->setFillColor(sf::Color::Black);
    rect->setPosition(x, y);
}

Platform::Platform(int x, int y, int width, int height, sf::Color fillColor, sf::Color outlineColor) : CObject(x, y, width, height)
{
    rect = new sf::RectangleShape;
    rect->setSize(sf::Vector2f(width, height));
    rect->setOutlineColor(outlineColor);
    rect->setFillColor(fillColor);
    rect->setPosition(x, y);
}

Platform::~Platform() {
}

void Platform::setCollidable(bool collidable) {
    this->collidable = collidable;
}

bool Platform::isCollidable() const {
    return collidable;
}

sf::Drawable* Platform::getDrawForm()
{
    return rect;
}

