#pragma once
#ifndef SCENE_H
#define SCENE_H

class Application;
class GameState;

class Scene {
public:
    Scene();
    Scene(Application* app, GameState* gameState);
    virtual ~Scene();
    void init();
    void initMainMenu();
    void initNewGame();
    void generateLevel();
private:
    Application* app;
    GameState* gameState;
};

#endif /* SCENE_H */

