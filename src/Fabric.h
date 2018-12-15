#ifndef FABRIC_H
#define FABRIC_H

class Bullet;
class GameObjects;
class Resources;

class Fabric {
public:
    Fabric(GameObjects* go);
    Fabric(const Fabric& orig);
    virtual ~Fabric();

    Bullet* createBullet(float x, float y, int width, int height);
    
private:
    GameObjects* go;
    Resources* resources;
};

#endif /* FABRIC_H */

