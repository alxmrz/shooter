#include "CObject.h"

CObject::CObject(){}

CObject::CObject(int x, int y, int width, int height):
        x(x), 
        y(y), 
        width(width), 
        height(height)
{
    
}

bool CObject::collideRect(CObject obj)
{

}
bool CObject::collidePoint(int x, int y)
{
    return  x <= this->x + this->width &&
            x >= this->x &&
            y <= this->y + this->height && 
            y >= this->y;
}

sf::Drawable* CObject::getDrawForm(){
    
}


