#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

class Unit;
class CObject;
class Fabric;
class Window;
class Text;
class GameState;

struct GameObjects {
    GameObjects(GameState* gs);
    virtual ~GameObjects();
    
    std::vector<CObject*> buttons;
    std::vector<CObject*> background;
    std::map<int, CObject*> borders;
    std::vector<CObject*> bullets;
    std::vector<CObject*> playable;
    std::vector<CObject*> crystals;
    std::vector<CObject*> hearts;
    
    GameState* gs;
    Unit* player;
    Fabric* fabric;
    
    sf::Sprite* backgroundSprite;
    sf::Sprite* heartSprite;
    sf::Sprite* crystalSprite;
    Text* text;
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
    void drawPlayerUi(Window* window);
};


#endif /* GAMEOBJECTS_H */

