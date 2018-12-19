#include <vector>
#include <cmath>
#include "CObject.h"
#include "GameObjects.h"
#include "Window.h"

CObject::CObject()
{
}

CObject::CObject(int x, int y, int width, int height) :
x(x),
y(y),
width(width),
height(height)
{
}

CObject::CObject(GameObjects* go, int x, int y, int width, int height) :
go(go),
x(x),
y(y),
width(width),
height(height)
{
}

CObject::~CObject()
{
}

bool CObject::collideRect(CObject* obj)
{
    //TODO: is there more elegant and more easy way?
    
    std::vector<std::vector<float>> objPoints = obj->getBorderPoints();
    for (unsigned i = 0; i < objPoints.size(); i++) {
        if (this->collidePoint(objPoints[i][0], objPoints[i][1])) {
            return true;
        }
    }

    std::vector<std::vector<float>> thisPoints = this->getBorderPoints();
    for (unsigned i = 0; i < thisPoints.size(); i++) {
        if (obj->collidePoint(thisPoints[i][0], thisPoints[i][1])) {
            return true;
        }
    }

    return false;
}

bool CObject::collidePoint(int x, int y)
{
    return x <= this->x + this->width - 1 &&
            x >= this->x + 1 &&
            y <= this->y + this->height - 1 &&
            y >= this->y + 1;
}

sf::Drawable* CObject::getDrawForm()
{
    return NULL;
}

float CObject::getX()
{
    return this->x;
}

float CObject::getY()
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

std::vector<std::vector<float>> CObject::getBorderPoints()
{
    std::vector<std::vector<float>> points;
    // TODO: dirty and complicated view, need to refactor
    std::vector<float> first = {getX(), getY()};
    std::vector<float> second = {getX() + getWidth(), getY() + getHeight()};
    std::vector<float> third = {getX(), getY() + getHeight()};
    std::vector<float> fourth = {getX() + getWidth(), getY()};

    std::vector<float> fifth = {getX() + getWidth() / 2, getY()};
    std::vector<float> sixth = {getX(), getY() + getHeight() / 2};
    std::vector<float> seventh = {getX() + getWidth() / 2, getY() + getHeight()};
    std::vector<float> eight = {getX() + getWidth(), getY() + getWidth() / 2};

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

void CObject::draw(Window* window, float dt)
{
    window->draw(*getDrawForm());
}

bool CObject::collideObjectAfterMove(float x, float y)
{
    /**
     * TODO: need to fix a bug.
     * When a user coords is, for exmp, (140, 450) it will not see 
     * an object in (150, 400) while jumping, because for collision will be
     * taken object in (100, 400), but it is wrong, 
     * when from the left of a ground none of objects exists
     * 
     *       [shooter] <---- in the position groundObject will not be collided
     * [nullptr][groundObject]
     *       [shooter] <---- in the position groundObject will not be collided 
     */
    x = std::ceil(x);
    y = std::ceil(y);
    float nearX;
    float nearY;
    if (x <= 0.f ) {
         nearX = nearTopLeft(this->x + x);  
    } else if (x > 0.f) {
         nearX = nearDownRight(this->x + x);      
    } 
    
    if (y <= 0.f) {
        nearY = nearTopLeft(this->y + y);
    } else if (y > 0.f) {
        nearY = nearDownRight(this->y + y);
    }
    
    if (go->backgrounds[nearX][nearY] != nullptr) {
        CObject* nearest = go->backgrounds[nearX][nearY];
        
        CObject collider = CObject(
                getX() + x,
                getY() + y,
                getWidth(),
                getHeight()
                );
        if (collider.collideRect(nearest)) {
            return true;
        }       
    }

    return false;
}

int CObject::nearTopLeft(int current)
{
    int decimalResidue = current % 100;
    if (decimalResidue >= 50) {
        return std::ceil(current / 100.f) * 100 - 50;
    } else {
        return std::floor(current / 100.f) * 100;
    }
}

int CObject::nearDownRight(int current)
{
    int decimalResidue = current % 100;
    if (decimalResidue > 50) {
        return std::ceil(current / 100.f) * 100;
    } else {
        return std::floor(current / 100.f) * 100 + 50;
    }
}