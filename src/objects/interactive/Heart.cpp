#include <SFML/Graphics.hpp>
#include "Heart.h"
#include "../../CObject.h"

Heart::Heart() {
}

Heart::Heart(const Heart& orig) {
}

Heart::Heart(sf::Texture* texture, int x, int y, int width, int height) : CObject(x, y, width, height)
{
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    sprite->setPosition(x, y);
    sprite->setScale(0.8f, 0.8f);
    sprite->setTexture(*texture);
}

Heart::~Heart() {
}

sf::Drawable* Heart::getDrawForm()
{
    return sprite;
}