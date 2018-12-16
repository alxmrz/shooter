#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <vector>
#include <SFML/Graphics.hpp>

class Shooter;
class CObject;
class Fabric;
class Window;

struct GameObjects {
    GameObjects();
    virtual ~GameObjects();
    
    std::vector<CObject*> buttons;
    std::vector<CObject*> background;
    std::vector<CObject*> bullets;
    std::vector<CObject*> playable;
    std::vector<CObject*> crystals;
    
    Shooter* player;
    Fabric* fabric;
    
    /**
     * @var Sprite for drawing
     */
    sf::Sprite* backgroundSprite;
    
    /**
     * Texture for sprite (Texture loads image). Sprite contains the texture
     */
    sf::Texture* texture;
    
    /**
     * Get all objects of the game.
     * Not used. Bad implementation. 
     * 
     * TODO: fix and use in Window
     * 
     * @return 
     */
    std::vector<CObject*>* all();
    
    /**
     * Delete all game objects
     */
    void reset();
    
    void draw(Window* window, float dt);
    void correctBackgroundSprite();
};


#endif /* GAMEOBJECTS_H */

