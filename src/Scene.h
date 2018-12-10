#ifndef SCENE_H
#define SCENE_H

class Application;
class GameState;

class Scene {
public:
    Scene();
    Scene(Application* app, GameState* gameState);
    virtual ~Scene();
    
    /**
     * Init objects for main menu
     */
    void initMainMenu();
    
    /**
     * Init objects for new game
     */
    void initNewGame();
    
    /**
     * Generate level from string. Level string is inside the method until
     * count of levels is one
     */
    void generateLevel();
private:
    /**
     * Current instance of the application
     */
    Application* app;
    
    /**
     * Current game state instance
     */
    GameState* gameState;
};

#endif /* SCENE_H */

