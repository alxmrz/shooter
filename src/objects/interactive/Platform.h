#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>
#include "../../CObject.h"

class Platform: public CObject {
public:
    Platform();
    Platform(const Platform& orig);
    Platform(int x, int y, int width, int height);
    virtual ~Platform();
    
    sf::Drawable* getDrawForm() override;
private:
    sf::RectangleShape* rect;
};

#endif /* PLATFORM_H */

