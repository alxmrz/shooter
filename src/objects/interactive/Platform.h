#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>
#include "../../CObject.h"

class Platform: public CObject {
public:
    Platform();
    Platform(const Platform& orig);
    Platform(int x, int y, int width, int height);
    Platform(int x, int y, int width, int height, sf::Color fillColor, sf::Color outlineColor);
    virtual ~Platform();
    
    void setCollidable(bool collidable);
    bool isCollidable() const;
    
    sf::Drawable* getDrawForm() override;
private:
    sf::RectangleShape* rect;
    bool collidable = true;
};

#endif /* PLATFORM_H */

