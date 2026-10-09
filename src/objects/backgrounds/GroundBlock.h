#ifndef GROUNDBLOCK_H
#define GROUNDBLOCK_H

#include <SFML/Graphics.hpp>
#include "../../CObject.h"

class GroundBlock : public CObject {
public:
    GroundBlock();
    GroundBlock(const GroundBlock& orig);
    GroundBlock(int x, int y, int width, int height);
    virtual ~GroundBlock();
    
    void update(Event event) override;
    void draw(Window* window, float dt) override;
    sf::Drawable* getDrawForm() override;
private:
    sf::RectangleShape* rect;
};

#endif /* GROUNDBLOCK_H */
