#ifndef FABRIC_H
#define FABRIC_H

#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Bullet;
class GameObjects;
class Resources;
class Unit;
class Shooter;
class Ground;
class Crystal;
class Ammunition;
class Heart;
class CObject;
class Button;
class Text;

class Fabric {
public:
    Fabric(GameObjects* go);
    Fabric(const Fabric& orig);
    virtual ~Fabric();

    Bullet* createBullet(Shooter* shooter, float x, float y, int width, int height);
    Shooter* createShooter(float x, float y, int width, int height);
    Ground* createGround(float x, float y, int width, int height);
    Crystal* createCrystal(float x, float y, int width, int height);
    Ammunition* createAmmo(float x, float y, int width, int height);
    Heart* createHeart(float x, float y, int width, int height);
    CObject* createBorder(float x, float y, int width, int height);
    Text* createText(std::string text, int x, int y);
    Button* createButton(std::string id, std::string text, float x, float y, int width, int height);
    sf::Sprite* createSprite(std::string type, float x, float y);
    sf::Sound* getBackgroundSound();
    
private:
    GameObjects* go;
    Resources* resources;
};

#endif /* FABRIC_H */

