#ifndef FABRIC_H
#define FABRIC_H

#include <string>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Bullet;
class GameObjects;
class Resources;
class Unit;
class Shooter;
class Ground;
class GroundBlock;
class Crystal;
class Ammunition;
class Platform;
class Heart;
class Tree;
class CObject;
class Button;
class Text;
class Aim;
class Dialogue;
class Flag;

class Fabric {
public:
    Fabric(GameObjects* go);
    Fabric(const Fabric& orig);
    virtual ~Fabric();

    Bullet* createBullet(Shooter* shooter, float x, float y, int width, int height);
    Shooter* createShooter(float x, float y, int width, int height);
    Ground* createGround(float x, float y, int width, int height);
    Crystal* createCrystal(float x, float y, int width, int height);
    Aim* createAim(float x, float y, int width, int height);
    Ammunition* createAmmo(float x, float y, int width, int height);
    Heart* createHeart(float x, float y, int width, int height);
    Platform* createPlatform(float x, float y, int width, int height);
    GroundBlock* createGroundBlock(float x, float y, int width, int height);
    Flag* createFlag(float x, float y, int width, int height);
    Tree* createTree(float x, float y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength);
    Tree* createTree(float x, float y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength, int branchStep);
    Tree* createTree(float x, float y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength, int branchStep, int topBranchSide);
    Tree* createTree(float x, float y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength, int branchStep, int topBranchSide, int topBranchLength);
    CObject* createBorder(float x, float y, int width, int height);
    Text* createText(std::string text, int x, int y);
    Button* createButton(std::string id, std::string text, float x, float y, int width, int height);
    Dialogue* createDialogue(std::string type);
    virtual Dialogue* createDialogue(std::vector<std::string> fragments);
    virtual std::vector<std::string> getDialogueFragments(std::string type);
    sf::Sprite* createSprite(std::string type, float x, float y);
    sf::Sound* getBackgroundSound();
    
protected:
    /**
     * Construct a factory without loading resources, for lightweight test
     * doubles that override the dialogue creation methods.
     */
    Fabric();

private:
    GameObjects* go;
    Resources* resources;

    void scaleFlagSprite(sf::Sprite* sprite, int width, int height);
};

#endif /* FABRIC_H */

