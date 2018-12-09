#include "CObject.h"
#include "GameObjects.h"
#include "Window.h"
#include <vector>

CObject::CObject(){}

CObject::CObject(int x, int y, int width, int height):
        x(x), 
        y(y), 
        width(width), 
        height(height)
{
    
}

CObject::CObject(GameObjects* go, int x, int y, int width, int height):
        go(go),
        x(x), 
        y(y), 
        width(width), 
        height(height)
{
    
}

bool CObject::collideRect(CObject* obj)
{
    std::vector<std::vector<int>> objPoints = obj->getBorderPoints();
    for (unsigned i = 0; i < objPoints.size(); i++) {
        if (this->collidePoint(objPoints[i][0], objPoints[i][1])) {
            return true;
        }
    }
    
    std::vector<std::vector<int>> thisPoints = this->getBorderPoints();
    for (unsigned i = 0; i < thisPoints.size(); i++) {
        if (obj->collidePoint(thisPoints[i][0], thisPoints[i][1])) {
            return true;
        }
    }
    
    return false;
}
bool CObject::collidePoint(int x, int y)
{
    return  x <= this->x + this->width -1 &&
            x >= this->x +1 &&
            y <= this->y + this->height - 1 && 
            y >= this->y +1;
}

sf::Drawable* CObject::getDrawForm(){

}

int CObject::getX()
{
    return this->x;
}

int CObject::getY()
{
    return this->y;
}

int CObject::getWidth()
{
    return this->width;
}

int CObject::getHeight()
{
    return this->height;
}

std::vector<std::vector<int>> CObject::getBorderPoints()
{
    std::vector<std::vector<int>> points;
    
    std::vector<int> first  = {getX(), getY()};
    std::vector<int> second = {getX() + getWidth(), getY() + getHeight()};
    std::vector<int> third  = {getX(), getY() + getHeight()};
    std::vector<int> fourth = {getX() + getWidth(), getY()};
    
    std::vector<int> fifth  = {getX() + getWidth()/2, getY()};
    std::vector<int> sixth = {getX(), getY() + getHeight()/2};
    std::vector<int> seventh  = {getX() + getWidth()/2, getY() + getHeight()};
    std::vector<int> eight = {getX() + getWidth(), getY() + getWidth()/2};
    
    points.push_back(first);
    points.push_back(second);
    points.push_back(third);
    points.push_back(fourth);
    
    points.push_back(fifth);
    points.push_back(sixth);
    points.push_back(seventh);
    points.push_back(eight);
    
    return points; 
}

void CObject::draw(Window* window)
{
    window->draw(*getDrawForm());
}

bool CObject::collideObjectAfterMove(int x, int y)
{
    for (auto* obj: go->background)
    {
        CObject* collider = new CObject(
                getX() + x, 
                getY()+ y, 
                getWidth(), 
                getHeight()
        );
        if (collider->collideRect(obj))
        {
            return true;
            break;
        }
        delete collider;
    }
    
    return false;
}