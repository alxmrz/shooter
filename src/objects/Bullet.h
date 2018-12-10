#ifndef BULLET_H
#define BULLET_H

#include <string>
#include "../CObject.h"

namespace sf {
    class Drawable;
}
class GameObjects;

class Bullet : public CObject {
public:
    Bullet();
    Bullet(GameObjects* go, int x, int y, int width, int height);
    Bullet(const Bullet& orig);
    virtual ~Bullet();

    sf::Drawable* getDrawForm() override;
    bool move(int x, int y);
    void setDirection(std::string direction);
private:
    sf::CircleShape* shape;
    std::string direction;
    std::vector<int> endPosition;

    bool collidePlayableAfterMove(int x, int y);
};

#endif /* BULLET_H */

