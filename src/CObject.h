#ifndef COBJECT_H
#define COBJECT_H
#include <SFML/Graphics.hpp>

class CObject
{
public:
    CObject();
    CObject(int x, int y, int width, int height);
    bool collideRect(CObject obj);
    bool collidePoint(int x, int y);
    virtual sf::Drawable* getDrawForm();
    
protected:
    int x;
    int y;
    int width;
    int height;
};

#endif /* COBJECT_H */