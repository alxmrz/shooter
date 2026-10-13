#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "CObject.h"

#include "Event.h"
#include "GameObjects.h"
#include "Window.h"

namespace {
struct PointInside {
    CObject* target;

    bool operator()(const std::vector<float>& point) const {
        return target->collidePoint(static_cast<int>(point[0]), static_cast<int>(point[1]));
    }
};

bool anyPointInside(const std::vector<std::vector<float>>& points, CObject* target) {
    return std::any_of(points.begin(), points.end(), PointInside{target});
}

bool borderBlocks(int borderX, CObject* border, CObject& collider, int checkPoint) {
    const int borderRight = borderX + border->getWidth();
    bool collideX = (checkPoint >= borderX && checkPoint < borderRight - 1) ||
                    (checkPoint + collider.getWidth() > borderX &&
                     checkPoint + collider.getWidth() < borderRight);
    return collideX && collider.collideRect(border);
}

struct BorderCollision {
    CObject* collider;
    int checkPoint;

    bool operator()(const std::pair<const int, CObject*>& entry) const {
        return borderBlocks(entry.first, entry.second, *collider, checkPoint);
    }
};
}

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
    return anyPointInside(obj->getBorderPoints(), this) ||
           anyPointInside(this->getBorderPoints(), obj);
}

bool CObject::collidePoint(int x, int y)
{
    return x <= this->x + this->width - 1 &&
            x >= this->x + 1 &&
            y <= this->y + this->height - 1 &&
            y >= this->y + 1;
}

void CObject::update(Event event)
{
}

void CObject::destroy()
{
    std::vector<CObject*>::iterator iter = std::find(go->all.begin(), go->all.end(), this);

    if (iter == go->all.end()) {
        return;
    }

    go->all.erase(iter);
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
    CObject collider = movedCollider(x, y);
    int checkPoint = collisionCheckPoint(collider, x, y);

    return collidesAnyBorder(collider, checkPoint);
}

CObject CObject::movedCollider(float x, float y)
{
    float stepX = (x < 0) ? std::floor(x) : std::ceil(x);

    return CObject(getX() + stepX, getY() + y, getWidth(), getHeight());
}

int CObject::collisionCheckPoint(CObject& collider, float x, float y)
{
    if (x <= 0 || y != 0) {
        return static_cast<int>(collider.getX());
    }

    return static_cast<int>(collider.getX() + collider.getWidth());
}

bool CObject::collidesAnyBorder(CObject& collider, int checkPoint)
{
    return std::any_of(go->borders.begin(), go->borders.end(),
                       BorderCollision{&collider, checkPoint});
}

void CObject::setCoords(float x, float y)
{
    this->x = x;
    this->y = y;
}

void CObject::setMainSprite(sf::Sprite* sprite)
{
    mainSprite = sprite;
}