#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

#include "Event.h"

class Unit;
class CObject;
class Fabric;
class Window;
class Text;
class Button;
class GameState;
class Aim;
class Dialogue;

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
    Aim* aim;
    Dialogue* dialogue;
    
    sf::Sprite* backgroundSprite;
    sf::Sprite* groundSprite;
    sf::Sprite* heartSprite;
    sf::Sprite* crystalSprite;
    sf::Sprite* ammoSprite;
    
    sf::RenderTexture* grounds;
    sf::Sprite* groundsSprite = nullptr;
    
    Text* cristalCountText;
    Text* ammoCountText;
    
    // UI элементы для экрана смерти
    Text* gameOverText;
    Button* restartButton;
    sf::RectangleShape* gameOverContainer;
    
    /**
     * Texture for sprite (Texture loads image). Sprite contains the texture
     */
    sf::Texture* texture;
    
    /**
     * Флаг, что игрок умер и игра остановлена
     */
    bool gameOver = false;
    
    /**
     * Delete all game objects
     */
    void clear();
    
    void draw(Window* window, float dt);
    void correctBackgroundSprite();
    void drawPlayerUi(Window* window);
    
    /**
     * Draw the dialogue overlay in screen space above the game world.
     */
    void drawDialogue(Window* window);
    
    /**
     * Update the current frame: while the dialogue is open only the dialogue
     * advances, otherwise the game world updates.
     */
    void update(Event event, float dt);
    
    /**
     * Update every game world object.
     */
    void updateWorld(Event event);
    
    /**
     * @return true while the intro dialogue is open
     */
    bool isDialogueOpen();
    
    /**
     * Advance the dialogue on a Space press.
     */
    void advanceDialogue();
    
    /**
     * Create and open the intro dialogue at the start of a new game.
     */
    void startIntroDialogue();
    
    /**
     * Показать экран смерти
     */
    void showGameOverScreen();
    
    /**
     * Скрыть экран смерти
     */
    void hideGameOverScreen();
    
    /**
     * Перезапустить игру
     */
    void restartGame();
};


#endif /* GAMEOBJECTS_H */

