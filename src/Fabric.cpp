#include <string>
#include "Fabric.h"
#include "GameObjects.h"
#include "Resources.h"
#include "objects/Bullet.h"
#include "objects/Shooter.h"
#include "objects/Ground.h"
#include "ui/Button.h"
#include "ui/Text.h"


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

Shooter* Fabric::createShooter(float x, float y, int width, int height)
{
    Shooter* shooter = new Shooter(go, x, y, width, height);
    shooter->setMainTexture( resources->getTexture("shooter")); 
    shooter->setExplosionTexture( resources->getTexture("explosion"));
    
    return shooter;
}

Ground* Fabric::createGround(float x, float y, int width, int height)
{
    return new Ground(resources->getTexture("ground"), x, y, width, height);
}

Text* Fabric::createText(std::string text, int x, int y)
{
    return new Text(text, resources->getFont("arial"), x, y);
}

Button* Fabric::createButton(std::string id, std::string text, float x, float y, int width, int height)
{
    Button* button = new Button(x, y, width, height);
    button->setId(id);
    button->setText(createText(text, x, y));
    
    return button;
}
