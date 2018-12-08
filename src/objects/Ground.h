#ifndef GROUND_H
#define GROUND_H

#include "../CObject.h"

class Ground: public CObject {
public:
    Ground();
    Ground(const Ground& orig);
    Ground(int x, int y, int width, int height);
    virtual ~Ground();
    
    sf::RectangleShape* shape;
    sf::Sprite* sprite;
    sf::Texture* texture;
    sf::Drawable* getDrawForm() override;
    void move(int x, int y);
    void setFormColor(sf::Color);
private:

};

#endif /* GROUND_H */

