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
    rect->setOutlineColor(sf::Color::Red);
    rect->setFillColor(sf::Color::Black);
    rect->setPosition(x, y);
}

Platform::~Platform() {
}

sf::Drawable* Platform::getDrawForm()
{
    return rect;
}

