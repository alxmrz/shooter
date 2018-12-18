#include <string>
#include <iostream>
#include <exception>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Resources.h"


Resources::Resources()
{   
}

Resources::Resources(const Resources& orig)
{
}

Resources::~Resources()
{

}

void Resources::load()
{
    loadTextures();
    loadFonts();
    loadSounds();
}

void Resources::loadTextures()
{
    sf::Texture* texture;
    for (auto it = texturesLoadQueue.begin(); it != texturesLoadQueue.end(); it++) {
        texture = new sf::Texture();
        std::cout <<it->second << std::endl;
        if (!texture->loadFromFile(it->second)) {
            std::cout << "Texture" + it->second + " is not found" << std::endl;
            throw;
        }
        
        textures[it->first] = texture;
    }
}

void Resources::loadFonts()
{
    sf::Font* font;
    for (auto it = fontsLoadQueue.begin(); it != fontsLoadQueue.end(); it++) {
        font = new sf::Font();
        if (!font->loadFromFile(it->second)) {
            std::cout << "Texture" + it->second + " is not found" << std::endl;
            throw;
        }
        
        fonts[it->first] = font;
    }
}

void Resources::loadSounds()
{
    sf::SoundBuffer* soundBuffer;
    sf::Sound* sound;
    for (auto it = soundsLoadQueue.begin(); it != soundsLoadQueue.end(); it++) {
        soundBuffer = new sf::SoundBuffer();
        if (!soundBuffer->loadFromFile(it->second)) {
            std::cout << "Texture" + it->second + " is not found" << std::endl;
            throw;
        }
        sound = new sf::Sound;
        sound->setBuffer(*soundBuffer);
        
        sounds[it->first] = sound;
    }
}

sf::Texture* Resources::getTexture(std::string type)
{
    if (textures.find(type) != textures.end()) {
        return textures[type];
    } else {
        std::cout << "Not found texture type " + type;
        throw;
    }
}

sf::Font* Resources::getFont(std::string type)
{
    if (fonts.find(type) != fonts.end()) {
        return fonts[type];
    } else {
        std::cout << "Not found texture type " + type;
        throw;
    }
}

sf::Sound* Resources::getSound(std::string type)
{
    if (sounds.find(type) != sounds.end()) {
        return sounds[type];
    } else {
        std::cout << "Not found texture type " + type;
        throw;
    }
}

