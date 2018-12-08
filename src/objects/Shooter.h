#ifndef SHOOTER_H
#define SHOOTER_H

#include "../CObject.h"
#include "../GameObjects.h"

struct GameObjects;

class Shooter: public CObject 
{
public:
    Shooter();
    Shooter(GameObjects* go, int x, int y, int width, int height);
    Shooter(const Shooter& orig);
    virtual ~Shooter();
    
    sf::Sprite* sprite;
    sf::Texture* texture;
    sf::Drawable* getDrawForm() override;
    void move(int x, int y);
    void setFormColor(sf::Color);
private:

};

#endif /* SHOOTER_H */

