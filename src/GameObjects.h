#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <vector>
#include <map>
#include <string>
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
    
    /**
     * True while the currently open dialogue is the flag-completion one, so
     * closing it marks the stage as completed.
     */
    bool flagDialogue = false;
    
    /**
     * True while the currently open dialogue is the death dialog with choices.
     */
    bool deathDialogue = false;
    
    /**
     * Set when the flag-completion dialogue has been closed and the game must
     * start the next stage; consumed by GameState.
     */
    bool stageRestartPending = false;
    
    sf::Sprite* backgroundSprite;
    sf::Sprite* groundSprite;
    sf::Sprite* heartSprite;
    sf::Sprite* crystalSprite;
    sf::Sprite* ammoSprite;
    sf::Sprite* flagSprite;
    
    sf::RenderTexture* grounds;
    sf::Sprite* groundsSprite = nullptr;
    
    Text* cristalCountText;
    Text* ammoCountText;
    Text* flagCountText;
    
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
     * Capture a flag: count it and open the flag-completion dialogue.
     */
    void collectFlag();
    
    /**
     * Create and open the flag-completion dialogue from the current stage text.
     */
    void startFlagDialogue();
    
    /**
     * Replace the open dialogue with the given one, tracking whether it is the
     * flag-completion dialogue. An empty dialogue is discarded and closes it.
     */
    void startDialogue(Dialogue* newDialogue, bool isFlag);
    
    /**
     * @return true once, when a completed stage is waiting to be restarted
     */
    bool consumeStageRestart();
    
    /**
     * @return true while the flag-completion dialogue is the open one
     */
    bool isFlagDialogue() const;
    
    /**
     * Open the death dialog (same look as the intro) with Yes/No action buttons.
     */
    void openDeathDialogue();
    
    /**
     * @return true while the death dialog with choices is open
     */
    bool isDeathDialogueOpen();
    
    /**
     * Index of the death-dialog choice at screen pixel (x, y), or -1.
     */
    int deathChoiceAt(int x, int y);
    
    /**
     * Build the intro fragment list for a run. At the very start (no flags) the
     * base intro is shown; once at least one flag was captured, the base intro
     * is replaced by a short continuation that reports the progress.
     */
    static std::vector<std::string> buildIntroFragments(
        const std::vector<std::string>& firstStart,
        const std::vector<std::string>& continuation,
        int flags,
        int stage);
    
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

private:
    void drawWorld(Window* window, float dt);
    void drawBackground(Window* window);
    void drawGrounds(Window* window);
    void ensureGroundsSprite();
    void buildGroundsSprite();
    void drawBackgroundTiles();
    void drawBackgroundTile(unsigned i);
    void drawVisibleObjects(Window* window, float dt);
    void drawObjectIfVisible(CObject* object, CObject& windowRect, Window* window, float dt);
    void drawButtons(Window* window, float dt);

    /**
     * Set the open dialogue and its flag-completion marker, opening it.
     */
    void beginDialogue(Dialogue* newDialogue, bool isFlag);
    
    /**
     * Advance the currently open dialogue and react to it closing.
     */
    void advanceOpenDialogue();
    
    /**
     * Mark the stage as completed when the flag dialogue just closed.
     */
    void markStageRestartIfFlagClosed();
    
    static std::vector<std::string> continuedIntro(
        const std::vector<std::string>& continuation, int flags, int stage);
    static std::string replaceProgressTokens(const std::string& line, int flags, int stage);
    static std::string replaceToken(const std::string& line, const std::string& token,
                                    const std::string& value);
};


#endif /* GAMEOBJECTS_H */

