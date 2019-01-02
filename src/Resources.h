#ifndef RESOURCES_H
#define RESOURCES_H

#include <exception>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ResourceNotLoaded : public std::exception 
{
public:

    ResourceNotLoaded(std::string const &message) : message(message) 
    {
    }

    virtual char const *what() const noexcept 
    {
        return message.c_str();
    }
private:
    std::string message;
};

class ResourceTypeNotFound : public std::exception 
{
public:

    ResourceTypeNotFound(std::string const &message) : message(message) 
    {
    }

    virtual char const *what() const noexcept 
    {
        return message.c_str();
    }
private:
    std::string message;
};


class Resources 
{
public:
    Resources();
    Resources(const Resources& orig);
    virtual ~Resources();
    
    sf::Texture* getTexture(std::string type);
    sf::Font* getFont(std::string type);
    sf::Sound* getSound(std::string type);
    
    void load();
private: 
    std::map<std::string, std::string> texturesLoadQueue {
        {"bullet", "resources/images/shell.png"},
        {"shooter", "resources/images/OpenGunnerHeroVer2.png"},
        {"explosion", "resources/images/oneshot.png"},
        {"ground", "resources/images/OpenGunnerForestTiles-1.png"},
        {"heart", "resources/images/heart.png"},
        {"crystal", "resources/images/crystal.png"},
        {"background", "resources/images/background.png"},
        {"ammo", "resources/images/ammo.png"},
    };
    
    std::map<std::string, std::string> fontsLoadQueue = {
        {"arial", "resources/fonts/arial.ttf"},
    };
    
    std::map<std::string, std::string> soundsLoadQueue = {
        {"explosion", "resources/audio/explosion_sound.wav"},
        {"background", "resources/audio/background_loop.wav"},
        {"jump", "resources/audio/jump_sound.wav"},
        {"shotgun", "resources/audio/shotgun_sound.wav"},
        {"crystal", "resources/audio/collecting_crystal.wav"},
    };
    
    std::map<std::string, sf::Texture*> textures;
    std::map<std::string, sf::Font*> fonts;
    std::map<std::string, sf::Sound*> sounds;
    
    void loadTextures();
    void loadFonts();
    void loadSounds();
};

#endif /* RESOURCES_H */

