#ifndef SCENE_H
#define SCENE_H


namespace tinyxml2 {
    class XMLElement;
}

class Application;
class GameState;
class Map;

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
private:
    /** Current instance of the application */
    Application* app;
    
    /** Current game state instance */
    GameState* gameState;
    
    /** Map generator*/
    Map* map;
};

#endif /* SCENE_H */

