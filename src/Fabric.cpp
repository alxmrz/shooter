#include "Fabric.h"
#include "GameObjects.h"
#include "Resources.h"
#include "objects/Bullet.h"


Fabric::Fabric(GameObjects* go):
go(go)
{
    resources = new Resources();
}

Fabric::Fabric(const Fabric& orig)
{
}

Fabric::~Fabric()
{
}

Bullet* Fabric::createBullet(float x, float y, int width, int height)
{
    return new Bullet(go, resources->getTexture("bullet"), x, y, width, height);
}

