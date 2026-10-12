#ifndef EXPLOSION_FRAMES_H
#define EXPLOSION_FRAMES_H

#include <vector>
#include <SFML/Graphics.hpp>

/**
 * Frames of the explosion animation from resources/images/oneshot.png.
 * Shared by every object that shows an explosion (for example, Shooter and Bullet).
 */
inline std::vector<sf::IntRect> explosionFrameRects()
{
    return {
        sf::IntRect(2, 0, 80, 75),
        sf::IntRect(80, 0, 80, 75),
        sf::IntRect(160, 0, 80, 75),
        sf::IntRect(240, 0, 80, 75),
        sf::IntRect(320, 0, 75, 75),
        sf::IntRect(400, 0, 0, 75),
    };
}

#endif /* EXPLOSION_FRAMES_H */
