#include <iostream>
#include <SFML/Graphics.hpp>
#include "Shooter.h"
#include "../CObject.h"
#include "../GameObjects.h"

Shooter::Shooter() {
}

Shooter::Shooter(const Shooter& orig) {
}

Shooter::Shooter(GameObjects* go, int x, int y, int width, int height): CObject(go, x, y, width, height)
{
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(24, 143, 50, 50));
    sprite->setPosition(x,y);
    texture = new sf::Texture();
    if (!texture->loadFromFile("images/OpenGunnerHeroVer2.png"))
    {
        throw;
        std::cout << "NO image found";
    }
    sprite->setTexture(*texture);
}

Shooter::~Shooter() {
}

void Shooter::move(int x, int y)
{
    this->x += x;
    this->y += y;
    this->sprite->setPosition(this->x, this->y);
}

sf::Drawable* Shooter::getDrawForm()
{
    return sprite;
}
