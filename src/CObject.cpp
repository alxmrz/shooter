#include <vector>
#include <cmath>
#include <iostream>
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
    delete mainSprite;
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
    return mainSprite;
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
    std::vector<float> eight = {getX() + getWidth(), getY() + getHeight() / 2};

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
    x = x < 0? std::floor(x) : std::ceil(x);
    std::ceil(y);
    
    CObject collider = CObject(
            getX() + x,
            getY() + y,
            getWidth(),
            getHeight()
            );
    int checkPoint = 0;
    if (x <= 0 || y < 0 || y > 0) {
        checkPoint = collider.getX();
    } else if ( x > 0) {
        checkPoint = collider.getX() + collider.getWidth();
    }

    for (auto it = go->borders.begin(); it != go->borders.end(); it++) {
        bool collideX = (checkPoint >= it->first && checkPoint < it->first + it->second->getWidth()-1) ||
        (checkPoint + collider.getWidth() > it->first && checkPoint + collider.getWidth() < it->first + it->second->getWidth());
        if (collideX && collider.collideRect(it->second)) {
            return true;
        }
    }

    return false;
}

void CObject::setMainSprite(sf::Sprite* sprite)
{
    mainSprite = sprite;
}