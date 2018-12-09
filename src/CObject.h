#ifndef COBJECT_H
#define COBJECT_H
#include <SFML/Graphics.hpp>
#include <vector>

class Window;
struct GameObjects;

class CObject
{
public:
    CObject();
    CObject(int x, int y, int width, int height);
    CObject(GameObjects* go, int x, int y, int width, int height);
    
    bool collideRect(CObject* obj);
    bool collidePoint(int x, int y);
    virtual sf::Drawable* getDrawForm();
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    virtual void draw(Window* window);
    std::vector<std::vector<int>> getBorderPoints();
    
protected:
    GameObjects* go;
    int x;
    int y;
    int width;
    int height;
    bool collideObjectAfterMove(int x, int y);
    
};

#endif /* COBJECT_H */