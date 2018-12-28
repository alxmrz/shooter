#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Application;
class CObject;
class GameObjects;

class GameState {
public:
    /**
     * @var Object for getting elapsed time after last frame
     */
    sf::Clock clock;
    
    /**
     * @var Object for ruling game camera.
     */
    sf::View* view;
    
    /**
     * @var game objects container
     */
    GameObjects* objects;

    GameState();
    GameState(Application* app);
    virtual ~GameState();
    
    /**
     * Update current game state every frame
     */
    void update();
    bool isGameStarted = false;
    bool isGamePaused = false;
    void playBackgroundMusic();
    void stopBackgroundMusic();
    void startNewGame();
    float getElapsedTime();
    void resetElapsedTime();
private:
    /* @var current application instance */
    Application* app;
    
    sf::Sound* backgroundMusic;
    
    /* Elapsed time from list frame*/
    float elapsedTime = 0.0;

    /**
     * Call gravity for objects
     */
    void causeGravity();
    
    /**
     * Move all bullets and deleted if the are ready
     */
    void updateBulletsState();
};

#endif