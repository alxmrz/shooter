#include <string>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "../GameObjects.h"

Bullet::Bullet() {
}

Bullet::Bullet(const Bullet& orig) {
}

Bullet::~Bullet() {
}

Bullet::Bullet(GameObjects* go, int x, int y, int width, int height): CObject(go, x, y, width, height)
{
    this->shape = new sf::CircleShape(width/2);
    this->shape->setFillColor(sf::Color::Red);
    this->shape->setPosition(x, y);
    
    endPosition.push_back(x+500);
    endPosition.push_back(y);
}

sf::Drawable* Bullet::getDrawForm(){
    return shape;
}

bool Bullet::move(int x, int y)
{    
    if (direction == "right" && x < 0 || direction == "left")
    {
        x *= -1;
    }

    bool isEndPosition = endPosition[0] == this->x+x && endPosition[1] == this->y+y;
    if (!collidePlayableAfterMove(x, y) && !collideObjectAfterMove(x, y))
    {
        this->x += x;
        this->y += y;
        this->shape->setPosition(this->x, this->y);
        
        return true;
    }
    
    return false;
}

void Bullet::setDirection(std::string dir)
{
    direction = dir;
}

bool Bullet::collidePlayableAfterMove(int x, int y)
{
    for (unsigned i = 0; i < go->playable.size(); i++)
    {
        auto* obj = go->playable[i];
        CObject* collider = new CObject(
                getX() + x, 
                getY()+ y, 
                getWidth(), 
                getHeight()
        );
        if (collider->collideRect(obj))
        {
            go->playable.erase(go->playable.begin() + i);
            delete collider;
            delete this;
            return true;
        }
        
        delete collider;
    }
    
    return false;
}