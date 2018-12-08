#include <iostream>
#include <SFML/Graphics.hpp>
#include "Shooter.h"

Shooter::Shooter() {
}

Shooter::Shooter(const Shooter& orig) {
}

Shooter::Shooter(int x, int y, int width, int height): CObject(x, y, width, height)
{
    sprite = new sf::Sprite();
    sprite->setTextureRect(sf::IntRect(20, 140, 50, 50));
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

sf::Drawable* Shooter::getDrawForm()
{
    return sprite;
}
