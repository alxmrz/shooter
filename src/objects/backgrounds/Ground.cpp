#include <iostream>
#include <SFML/Graphics.hpp>
#include "Ground.h"
#include "../../Window.h"

Ground::Ground() : rect(nullptr)
{
}

Ground::Ground(const Ground& orig) : rect(nullptr)
{
}

Ground::Ground(int x, int y, int width, int height) : CObject(x, y, width, height)
{
    rect = new sf::RectangleShape;
    rect->setSize(sf::Vector2f(width, height));
    rect->setOutlineColor(sf::Color::Black);
    //rect->setFillColor(sf::Color(34, 139, 34)); // Dark green
    rect->setFillColor(sf::Color(100, 0, 0)); // Dark green
    rect->setPosition(x, y);
}


Ground::~Ground() {
    delete rect;
}

void Ground::update(Event event) {
    // Ground is static
}

void Ground::draw(Window* window, float dt) {
    if (rect) {
        window->draw(*rect);
    }
}

sf::Drawable* Ground::getDrawForm() {
    return rect;
}