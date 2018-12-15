#include <string>
#include <iostream>
#include <exception>
#include <SFML/Graphics.hpp>
#include "Resources.h"


Resources::Resources()
{
    bulletTexture = new sf::Texture();
    if (!bulletTexture->loadFromFile("resources/images/shell.png")) {
        std::cout << "Image resources/images/shell.png is not found" << std::endl;
        throw;
    }
}

Resources::Resources(const Resources& orig)
{
}

Resources::~Resources()
{
    delete bulletTexture;
}

sf::Texture* Resources::getTexture(std::string type)
{
    if (type == "bullet") {
        return bulletTexture;
    }
    
    throw;
}


