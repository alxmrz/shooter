#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Fabric.h"
#include "GameObjects.h"
#include "Resources.h"
#include "objects/Bullet.h"
#include "objects/Shooter.h"
#include "objects/Ground.h"
#include "objects/Crystal.h"
#include "ui/Button.h"
#include "ui/Text.h"


Fabric::Fabric(GameObjects* go):
go(go)
{
    resources = new Resources();
    resources->load();
}

Fabric::Fabric(const Fabric& orig)
{
}

Fabric::~Fabric()
{
}

Bullet* Fabric::createBullet(Shooter* shooter, float x, float y, int width, int height)
{
    Bullet* bullet = new Bullet(shooter, go, x, y, width, height);
    bullet->setMainTexture(resources->getTexture("bullet"));
    bullet->setExplosionSound(resources->getSound("explosion"));
    return bullet;
}

Shooter* Fabric::createShooter(float x, float y, int width, int height)
{
    Shooter* shooter = new Shooter(go, x, y, width, height);
    
    sf::Sprite* heartSprite = createSprite("heart", x, y);
    heartSprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    heartSprite->setScale(0.5f, 0.5f);

    shooter->setMainSprite(createSprite("shooter", x, y));
    shooter->setHeartSprite(heartSprite);
    shooter->setExplosionSprite(createSprite("explosion", x, y));
    shooter->setJumpSound(resources->getSound("jump"));
    shooter->setShotgunSound(resources->getSound("shotgun"));
    shooter->setCrystalSound(resources->getSound("crystal"));
    
    return shooter;
}

Ground* Fabric::createGround(float x, float y, int width, int height)
{
    return new Ground(resources->getTexture("ground"), x, y, width, height);
}
Crystal* Fabric::createCrystal(float x, float y, int width, int height)
{
    return new Crystal(resources->getTexture("crystal"), x, y, width, height);
}

CObject* Fabric::createBorder(float x, float y, int width, int height)
{
    return new CObject(go, x, y, width, height);
}

Text* Fabric::createText(std::string text, int x, int y)
{
    return new Text(text, resources->getFont("arial"), x, y);
}

sf::Sprite* Fabric::createSprite(std::string type, float x, float y)
{
    sf::Sprite* sprite = new sf::Sprite();
    sprite->setPosition(x, y);
    sprite->setTexture(*resources->getTexture(type));
    
    return sprite;
}

Button* Fabric::createButton(std::string id, std::string text, float x, float y, int width, int height)
{
    Button* button = new Button(x, y, width, height);
    button->setId(id);
    button->setText(createText(text, x, y));
    
    return button;
}

sf::Sound* Fabric::getBackgroundSound()
{
    return resources->getSound("background");
}