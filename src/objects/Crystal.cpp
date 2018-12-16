#include <SFML/Graphics.hpp>
#include "Crystal.h"
#include "../CObject.h"

Crystal::Crystal()
{    
}

Crystal::Crystal(const Crystal& orig)
{
}

Crystal::Crystal(sf::Texture* texture, int x, int y, int width, int height) : CObject(x, y, width, height)
{
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    sprite->setPosition(x, y);
    sprite->setScale(0.8f, 0.8f);
    sprite->setTexture(*texture);
}

Crystal::~Crystal()
{
}

sf::Drawable* Crystal::getDrawForm()
{
    return sprite;
}

