#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Event.h"
#include "StageProgress.h"

class Application;
class CObject;
class GameObjects;
class AI;

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
    void update(Event event);
    bool isGameStarted = false;
    bool isGamePaused = false;
    void playBackgroundMusic();
    void stopBackgroundMusic();
    void startNewGame();
    float getElapsedTime();
    void resetElapsedTime();
    
    /**
     * Number of the stage currently being played (starts at 1).
     */
    int getStage() const;
    
    /**
     * Total number of flags captured during this run.
     */
    int getFlags() const;
    
    /**
     * Register a captured flag: counts it and advances the stage.
     */
    void collectFlag();
    
    /**
     * Forget all run progress (stage and captured flags).
     */
    void resetProgress();
private:
    /* @var current application instance */
    Application* app;
    
    sf::Sound* backgroundMusic;
    
    /* Elapsed time from list frame*/
    float elapsedTime = 0.0;

    /* Stage and flag progress for the current run (memory only) */
    StageProgress progress;

    bool isGameplayRunning();
    void updateRunningGame(Event event);
    void updateNonDialogue(Event event);
    bool consumeStageRestart();
    void updateDialogue(Event event);
    void updateGameplay(Event event);
    void updateWorld(Event event);
    void updatePlayerDeath();
    bool isDeathAnimationFinished();
    bool isPlayerDeathPending();
    bool hasDeadPlayer();
};

#endif