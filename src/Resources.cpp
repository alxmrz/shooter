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
    
    shooterTexture = new sf::Texture();
    if (!shooterTexture->loadFromFile("resources/images/OpenGunnerHeroVer2.png")) {
        std::cout << "Image resources/images/OpenGunnerHeroVer2.png is not found";
        throw;
        
    }
    
    explosionTexture = new sf::Texture();
    if (!explosionTexture->loadFromFile("resources/images/oneshot.png")) {
        std::cout << "Image resources/images/shoneshotell.png is not found";
        throw;     
    }
    
    groundTexture = new sf::Texture();
    if (!groundTexture->loadFromFile("resources/images/ground.jpeg")) {
        std::cout << "Image images/ground.jpeg is not found" << std::endl;
        throw;
    }
    
    arialFont = new sf::Font;
    if (!arialFont->loadFromFile("resources/fonts/arial.ttf")) {
        std::cout << "Font fonts/arial.ttf not found!";
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
    } else if (type == "shooter") {
        return shooterTexture;
    } else if (type == "explosion") {
        return explosionTexture;
    } else if (type == "ground") {
        return groundTexture;
    }
    std::cout << "Not found texture type " + type;
    throw;
}

sf::Font* Resources::getFont(std::string type)
{
    if (type == "arial") {
        return arialFont;
    }
    std::cout << "Not found font type " + type;
    throw;
}


