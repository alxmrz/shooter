#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Fabric.h"
#include "GameObjects.h"
#include "Resources.h"
#include "objects/interactive/Bullet.h"
#include "objects/units/Unit.h"
#include "objects/units/Shooter.h"
#include "objects/backgrounds/Ground.h"
#include "objects/interactive/Crystal.h"
#include "objects/interactive/Heart.h"
#include "objects/interactive/Ammunition.h"
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
    sf::Sprite* sprite = createSprite("bullet", x, y);
    sprite->setTextureRect(sf::IntRect(0, 0, 18, 7));
    
    Bullet* bullet = new Bullet(shooter, go, x, y, width, height);
    bullet->setMainSprite(sprite);
    bullet->setExplosionSound(resources->getSound("explosion"));
    return bullet;
}

Crystal* Fabric::createCrystal(float x, float y, int width, int height)
{
    sf::Sprite* sprite = createSprite("crystal", x, y);
    sprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    sprite->setScale(0.8f, 0.8f);
    
    Crystal* crystal = new Crystal(x, y, width, height);
    crystal->setMainSprite(sprite);
    
    return crystal;
}

Ammunition* Fabric::createAmmo(float x, float y, int width, int height)
{
    sf::Sprite* sprite = createSprite("ammo", x, y);
    sprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    sprite->setScale(0.8f, 0.8f);
    
    Ammunition* ammo = new Ammunition(x, y, width, height);
    ammo->setMainSprite(sprite);
    
    return ammo;
}

Heart* Fabric::createHeart(float x, float y, int width, int height)
{
    sf::Sprite* sprite = createSprite("heart", x, y);
    sprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    sprite->setScale(0.8f, 0.8f);
    
    Heart* heart = new Heart(x, y, width, height);
    heart->setMainSprite(sprite);
    
    return heart;
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
    sf::Sprite* sprite = createSprite("ground", x, y);
    sprite->setTextureRect(sf::IntRect(0, 100, 50, 50));
    
    Ground* ground = new Ground(x, y, width, height);
    ground->setMainSprite(sprite);
    
    return ground;
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