#include <iostream>
#include <SFML/Graphics.hpp>
#include "GroundBlock.h"
#include "../../Window.h"

GroundBlock::GroundBlock() : rect(nullptr)
{
}

GroundBlock::GroundBlock(const GroundBlock& orig) : rect(nullptr)
{
}

GroundBlock::GroundBlock(int x, int y, int width, int height) : CObject(x, y, width, height)
{
    rect = new sf::RectangleShape;
    rect->setSize(sf::Vector2f(width, height));
    rect->setOutlineColor(sf::Color::Black);
    rect->setFillColor(sf::Color(34, 139, 34)); // Dark green
    rect->setPosition(x, y);
}

GroundBlock::~GroundBlock() {
    delete rect;
}

void GroundBlock::update(Event event) {
    // GroundBlock is static
}

void GroundBlock::draw(Window* window, float dt) {
    if (rect) {
        window->draw(*rect);
    }
}

sf::Drawable* GroundBlock::getDrawForm() {
    return rect;
}
