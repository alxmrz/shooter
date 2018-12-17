#ifndef RESOURCES_H
#define RESOURCES_H

#include <exception>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class NotLoadedTexture: public std::exception
{
public:
    NotLoadedTexture(std::string message);
};

class TypeNotFound: public std::exception
{
public:
    TypeNotFound(std::string message);
};


class Resources {
public:
    Resources();
    Resources(const Resources& orig);
    virtual ~Resources();
    
    sf::Texture* getTexture(std::string type);
    sf::Font* getFont(std::string type);
    sf::Sound* getSound(std::string type);
    
    void load();
private:
    sf::Texture* bulletTexture;
    sf::Texture* shooterTexture;
    sf::Texture* explosionTexture;
    sf::Texture* groundTexture;
    sf::Texture* heartTexture;
    sf::Texture* crystalTexture;
    sf::Texture* backgroundTexture;
    
    sf::Font* arialFont;
    
    sf::SoundBuffer* explosionSoundBuffer;
    sf::Sound* explosionSound;
    
    sf::SoundBuffer* backgroundSoundBuffer;
    sf::Sound* backgroundSound;
    
    sf::SoundBuffer* jumpSoundBuffer;
    sf::Sound* jumpSound;
    
    sf::SoundBuffer* shotgunSoundBuffer;
    sf::Sound* shotgunSound;
    
    sf::SoundBuffer* crystalSoundBuffer;
    sf::Sound* crystalSound;
    
    void loadTextures();
    void loadFonts();
    void loadSounds();
};

#endif /* RESOURCES_H */

