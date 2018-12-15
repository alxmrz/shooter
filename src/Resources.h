#ifndef RESOURCES_H
#define RESOURCES_H

#include <exception>
#include <SFML/Graphics.hpp>

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
private:
     /** Bullet texture */
    sf::Texture* bulletTexture;
    sf::Texture* shooterTexture;
    sf::Texture* explosionTexture;
};

#endif /* RESOURCES_H */

