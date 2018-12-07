#ifndef BUILDER_H
#define BUILDER_H
#include "CObject.h"
#include <SFML/Graphics.hpp>


class Builder: public CObject
{
public:
    sf::CircleShape* circle;
    Builder(int x, int y, int width, int height);
    Builder();
    sf::Drawable* getDrawForm();
    void move(int x, int y);
    void setFormColor(sf::Color);
};

#endif /* BUILDER_H */

