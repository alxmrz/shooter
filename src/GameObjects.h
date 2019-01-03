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
    std::map<int, CObject*> borders;
    std::vector<std::vector<int>> background;
    std::vector<CObject*> all;
    std::vector<std::vector<int>> globalTileIds;
    
    GameState* gs;
    Unit* player;
    Fabric* fabric;
    
    sf::Sprite* backgroundSprite;
    sf::Sprite* groundSprite;
    sf::Sprite* heartSprite;
    sf::Sprite* crystalSprite;
    sf::Sprite* ammoSprite;
    
    sf::RenderTexture* grounds;
    sf::Sprite* groundsSprite = nullptr;
    
    Text* cristalCountText;
    Text* ammoCountText;
    /**
     * Texture for sprite (Texture loads image). Sprite contains the texture
     */
    sf::Texture* texture;
    
    /**
     * Delete all game objects
     */
    void clear();
    
    void draw(Window* window, float dt);
    void correctBackgroundSprite();
    void drawPlayerUi(Window* window);
};


#endif /* GAMEOBJECTS_H */

