#ifndef AIM_H
#define AIM_H

#include <SFML/Graphics.hpp>
#include "../../CObject.h"

struct GameObjects;
class Window;

/**
 * Player crosshair. It follows the mouse cursor in world coordinates and
 * provides the direction the player shot travels toward.
 */
class Aim : public CObject {
public:
    Aim(GameObjects* go, float x, float y, int width, int height);
    virtual ~Aim();

    /**
     * Move the crosshair to a new world position.
     */
    void setTarget(float x, float y);

    float getTargetX();
    float getTargetY();

    /**
     * Normalized direction (unit vector) from the given point to the crosshair.
     * Returns a zero vector when the point coincides with the crosshair.
     */
    sf::Vector2f getDirectionFrom(float fromX, float fromY);

    void draw(Window* window, float dt) override;

private:
    sf::Vector2f target;

    void positionSprite();
    bool isHidden();
};

#endif /* AIM_H */
