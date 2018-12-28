#include <SFML/Graphics.hpp>
#include "Ammunition.h"
#include "../../CObject.h"

Ammunition::Ammunition() {
}

Ammunition::Ammunition(const Ammunition& orig) {
}

Ammunition::Ammunition(sf::Texture* texture, int x, int y, int width, int height) : CObject(x, y, width, height)
{
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    sprite->setPosition(x, y);
    sprite->setScale(0.8f, 0.8f);
    sprite->setTexture(*texture);
}

Ammunition::~Ammunition() {
}

sf::Drawable* Ammunition::getDrawForm()
{
    return sprite;
}
