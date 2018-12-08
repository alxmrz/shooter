#include <iostream>
#include <SFML/Graphics.hpp>
#include "Ground.h"

Ground::Ground() {
}

Ground::Ground(const Ground& orig) {
}

Ground::Ground(int x, int y, int width, int height): CObject(x, y, width, height)
{
    this->shape = new sf::RectangleShape(sf::Vector2f(width, height));
    this->shape->setPosition(x,y);
    shape->setFillColor(sf::Color(0,100,0));
    
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(0, 100, 50, 50));
    sprite->setPosition(x,y);
    texture = new sf::Texture();
    if (!texture->loadFromFile("images/ground.jpeg"))
    {
        throw;
        std::cout << "NO image found";
    }
    sprite->setTexture(*texture);
}

sf::Drawable* Ground::getDrawForm()
{
    return sprite;
}

Ground::~Ground() {
}

