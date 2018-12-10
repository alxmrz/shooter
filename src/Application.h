#pragma once

#include <SFML/Graphics.hpp>

class Event;
class Scene;
class Window;
class GameState;

class Application {
public:
    Application();
    ~Application();

    Window* window;
    Event* event;
    Scene* scene;

    int run();
private:
    int fps = 120;
    GameState* gameState;
};
