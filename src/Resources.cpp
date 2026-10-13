#include <string>
#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
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
        if (!texture->loadFromFile(it->second)) {
            throw ResourceNotLoaded("Texture " + it->second + " is not found");
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
            throw ResourceNotLoaded("Font " + it->second + " is not found");
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
             throw ResourceNotLoaded("Sound  " + it->second + " is not found");
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
        throw ResourceTypeNotFound("Not found texture type: " + type);
    }
}

sf::Font* Resources::getFont(std::string type)
{
    if (fonts.find(type) != fonts.end()) {
        return fonts[type];
    } else {
        throw ResourceTypeNotFound("Not found font type: " + type);
    }
}

sf::Sound* Resources::getSound(std::string type)
{
    if (sounds.find(type) != sounds.end()) {
        return sounds[type];
    } else {
        throw ResourceTypeNotFound("Not found sound type: " + type);
    }
}

std::vector<std::string> Resources::getDialogue(std::string type)
{
    std::ifstream file(dialoguePath(type));

    if (!file.is_open()) {
        return {};
    }

    return parseDialogueLines(file);
}

std::string Resources::dialoguePath(std::string type)
{
    auto it = dialogueLoadQueue.find(type);

    if (it == dialogueLoadQueue.end()) {
        return "";
    }

    return it->second;
}

std::vector<std::string> Resources::parseDialogueLines(std::istream& input)
{
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(input, line)) {
        addDialogueLine(lines, line);
    }

    return lines;
}

void Resources::addDialogueLine(std::vector<std::string>& lines, std::string line)
{
    if (isBlankDialogueLine(line)) {
        return;
    }

    lines.push_back(cleanDialogueLine(line));
}

bool Resources::isBlankDialogueLine(const std::string& line)
{
    return line.find_first_not_of(" \t\r\n") == std::string::npos;
}

std::string Resources::cleanDialogueLine(std::string line)
{
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    return line;
}

