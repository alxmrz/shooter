#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H


class Shooter;
class CObject;

struct GameObjects {
    GameObjects();

    std::vector<CObject*> buttons;
    std::vector<CObject*> background;
    std::vector<CObject*> bullets;
    std::vector<CObject*> playable;
    Shooter* player;

    virtual ~GameObjects();
    
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
};


#endif /* GAMEOBJECTS_H */

