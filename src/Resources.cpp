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
    for (auto it = texturesLoadQueue.begin(); it != texturesLoadQueue.end(); it++) {
        loadTexture(it->first, it->second);
    }
}

void Resources::loadTexture(const std::string& key, const std::string& path)
{
    sf::Texture* texture = new sf::Texture();

    if (!texture->loadFromFile(path)) {
        throw ResourceNotLoaded("Texture " + path + " is not found");
    }

    textures[key] = texture;
}

void Resources::loadFonts()
{
    for (auto it = fontsLoadQueue.begin(); it != fontsLoadQueue.end(); it++) {
        loadFont(it->first, it->second);
    }
}

void Resources::loadFont(const std::string& key, const std::string& path)
{
    sf::Font* font = new sf::Font();

    if (!font->loadFromFile(path)) {
        throw ResourceNotLoaded("Font " + path + " is not found");
    }

    fonts[key] = font;
}

void Resources::loadSounds()
{
    for (auto it = soundsLoadQueue.begin(); it != soundsLoadQueue.end(); it++) {
        loadSound(it->first, it->second);
    }
}

void Resources::loadSound(const std::string& key, const std::string& path)
{
    sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();

    if (!soundBuffer->loadFromFile(path)) {
        throw ResourceNotLoaded("Sound  " + path + " is not found");
    }

    sf::Sound* sound = new sf::Sound;
    sound->setBuffer(*soundBuffer);

    sounds[key] = sound;
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
    stripCarriageReturn(line);

    return line;
}

void Resources::stripCarriageReturn(std::string& line)
{
    if (hasTrailingCarriageReturn(line)) {
        line.pop_back();
    }
}

bool Resources::hasTrailingCarriageReturn(const std::string& line)
{
    return !line.empty() && line.back() == '\r';
}

