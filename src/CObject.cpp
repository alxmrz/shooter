#include "CObject.h"
#include <vector>

CObject::CObject(){}

CObject::CObject(int x, int y, int width, int height):
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

sf::Drawable* CObject::getDrawForm(){}

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
    
    std::vector<int> first  = {this->getX(), this->getY()};
    std::vector<int> second = {this->getX() + this->getWidth(), this->getY() + this->getHeight()};
    std::vector<int> third  = {this->getX(), this->getY() + this->getHeight()};
    std::vector<int> fourth = {this->getX() + this->getWidth(), this->getY()};
    
    std::vector<int> fifth  = {this->getX() + this->getWidth()/2, this->getY()};
    std::vector<int> sixth = {this->getX(), this->getY() + this->getHeight()/2};
    std::vector<int> seventh  = {this->getX() + this->getWidth()/2, this->getY() + this->getHeight()};
    std::vector<int> eight = {this->getX() + this->getWidth(), this->getY() + this->getWidth()/2};
    
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

