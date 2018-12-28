#include <iostream>
#include <SFML/Graphics.hpp>
#include "Ground.h"

Ground::Ground()
{
}

Ground::Ground(const Ground& orig)
{
}

Ground::Ground(sf::Texture* texture, int x, int y, int width, int height) : CObject(x, y, width, height)
{
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(0, 100, 50, 50));
    sprite->setPosition(x, y);
    sprite->setTexture(*texture);
}


Ground::~Ground()
{
}

sf::Drawable* Ground::getDrawForm()
{
    return sprite;
}
