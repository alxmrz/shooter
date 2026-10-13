#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Unit.h"
#include "../interactive/Crystal.h"
#include "../interactive/Heart.h"
#include "../interactive/Ammunition.h"
#include "../interactive/Platform.h"
#include "../interactive/Bullet.h"
#include "../interfaces/Movable.h"
#include "../../CObject.h"
#include "../../Window.h"
#include "../../GameObjects.h"
#include "../../Fabric.h"
#include "../../ui/Text.h"
#include "../../AI.h"

#include "../../Event.h"


Unit::Unit(GameObjects *go, float x, float y, int width, int height)
    : Movable(go, x, y, width, height) {
    ai = new AI(go, this);
}

Unit::~Unit() {
    delete ai;
}

void Unit::attack() {
}

void Unit::shiftSpritePositions() {
}

sf::Drawable *Unit::getDrawForm() {
    if (!dead) {
        return mainSprite;
    } else {
        return explosionSprite;
    }
}

bool Unit::move(float x, float y) {
    if (!Movable::move(x, y)) {
        return false;
    }

    shiftSpritePositions();
    collectItems();
    return resolvePlatformCollision(x, y);
}

void Unit::collectItems() {
    if (!isPlayer()) {
        return;
    }

    for (unsigned i = 0; i < go->all.size(); i++) {
        if (Crystal *obj = dynamic_cast<Crystal *>(go->all[i])) {
            if (collideRect(obj)) {
                crystals++;
                crystalSound->play();
                go->all.erase(go->all.begin() + i);

                break;
            }
        } else if (Ammunition *obj = dynamic_cast<Ammunition *>(go->all[i])) {
            if (collideRect(obj) && getAmmo() < 10) {
                ammo += 3;
                if (ammo > 10) ammo = 10;
                go->all.erase(go->all.begin() + i);

                break;
            }
        } else if (Heart *obj = dynamic_cast<Heart *>(go->all[i])) {
            if (collideRect(obj) && getHealth() < 3) {
                health++;
                go->all.erase(go->all.begin() + i);

                break;
            }
        }
    }
}

bool Unit::resolvePlatformCollision(float x, float y) {
    Platform* platform = findCollidingPlatform();
    if (platform == nullptr) {
        return true;
    }

    return applyPlatformCollision(platform, x, y);
}

Platform* Unit::findCollidingPlatform() {
    for (CObject* object : go->all) {
        Platform* platform = dynamic_cast<Platform*>(object);
        if (isSolidCollision(platform)) {
            return platform;
        }
    }

    return nullptr;
}

bool Unit::isSolidCollision(Platform* platform) {
    return platform != nullptr && platform->isCollidable() && collideRect(platform);
}

bool Unit::applyPlatformCollision(Platform* platform, float x, float y) {
    if (y != 0) {
        return applyVerticalCollision(platform, x, y);
    }

    return blockMoveHorizontal(x, y);
}

bool Unit::applyVerticalCollision(Platform* platform, float x, float y) {
    if (y > 0) {
        return landOnPlatform(platform);
    }

    return blockMoveUp(x, y);
}

bool Unit::landOnPlatform(Platform* platform) {
    this->y = platform->getY() - this->height;
    shiftSpritePositions();
    gravitationalVelocity = 0.0;
    gravitationalTime = 0.0;
    currentJumpHeight = 0.f;
    resetJumpCount();

    return false;
}

bool Unit::blockMoveUp(float x, float y) {
    revertMove(x, y);
    jumping = false;
    velocityHorizontal = 0.f;
    currentJumpHeight = 0.f;
    gravitationalVelocity = 0.0;
    gravitationalTime = 0.0;

    return true;
}

bool Unit::blockMoveHorizontal(float x, float y) {
    revertMove(x, y);

    return false;
}

void Unit::revertMove(float x, float y) {
    this->x -= x;
    this->y -= y;
    shiftSpritePositions();
}


void Unit::jump() {
    if (jumping) {
        return;
    }

    if (!canJump()) {
        return;
    }

    jumpCount++;

    if (currentJumpHeight == 0.f) {
        float jumpHeight = 100.f;
        if (jumpCount == 2) {
            jumpHeight = 60.f;
            onSecondJump();
        }
        jumpSound->play();
        currentJumpHeight = this->y - jumpHeight;
    }

    velocityHorizontal = -1.0f;
    jumping = true;
}

void Unit::gravitate() {
    float y = std::ceil(
        gravitationalTime * (gravitationalVelocity + elapsedTime * gravitationalAcceleration / 2.f)
    );

    falling = y > 0 && move(0.f, y);
    if (falling) {
        gravitationalVelocity += gravitationalTime * gravitationalAcceleration;
    } else {
        currentJumpHeight = 0.0;
        gravitationalVelocity = 0.0;
        gravitationalTime = 0.0;
        // Reset jump count when touching ground
        resetJumpCount();
        // Stop jumping state when on ground
        jumping = false;
        velocityHorizontal = 0.f;
    }
}

void Unit::update(Event event) {
    if (remove()) {
        // Не удаляем игрока, пока не показан экран завершения игры
        if (!(isPlayer() && !go->gameOver)) {
            destroy();
        }
    }

    think();

    if (jumping) {
        if (velocityHorizontal > -maxVelocity) {
            velocityHorizontal -= acceleration;
        }
        if (this->y > currentJumpHeight && move(0.f, velocityHorizontal)) {
        } else {
            jumping = false;
            velocityHorizontal = 0.f;
            currentJumpHeight = 0.f;
        }
    } else {
        gravitate();
    }

    if (isPlayer()) {
        if (event.IsKeyReleased()) {
            if (event.IsKeyBoardLeft()) {
                stopMoving();
            } else if (event.IsKeyBoardRight()) {
                stopMoving();
            }
        }

        if (isPlatformAffected) {
            jump();
            if (!isJumping()) {
                isPlatformAffected = false;
            }
        }
    }
}

void Unit::think() {
    ai->think();
}

bool Unit::isJumping() {
    return jumping;
}

bool Unit::isFalling() {
    return falling;
}

void Unit::stopJumping() {
    jumping = false;
    velocityHorizontal = 0.f;
}

void Unit::onSecondJump() {
    // Base implementation does nothing
}


bool Unit::isDead() {
    return dead;
}

bool Unit::remove() {
    return mustBeDeleted;
}

bool Unit::isPlayer() {
    return main;
}

int Unit::getHealth() {
    return health;
}

int Unit::getAmmo() {
    return ammo;
}

int Unit::getCrystals() {
    return crystals;
}

float Unit::getVelocityHorizontal() {
    return velocityHorizontal;
}

float Unit::getCurrentJumpHeight() {
    return currentJumpHeight;
}

void Unit::setMain(bool main) {
    this->main = main;
}

void Unit::setDead(bool dead) {
    this->dead = dead;
}

void Unit::setAimTarget(float x, float y) {
    aimTarget.x = x;
    aimTarget.y = y;
    aimTargetSet = true;
}

bool Unit::hasAimTarget() {
    return aimTargetSet;
}

float Unit::getAimTargetX() {
    return aimTarget.x;
}

float Unit::getAimTargetY() {
    return aimTarget.y;
}

void Unit::decreaseHealth() {
    health--;
}

void Unit::setMainSprite(sf::Sprite *mainSprite) {
    this->mainSprite = mainSprite;
}

void Unit::setHeartSprite(sf::Sprite *heartSprite) {
    this->heartSprite = heartSprite;
}

void Unit::setJumpSound(sf::Sound *jumpSound) {
    this->jumpSound = jumpSound;
}

void Unit::setCrystalSound(sf::Sound *crystalSound) {
    this->crystalSound = crystalSound;
}

void Unit::setShotgunSound(sf::Sound *shotgunSound) {
    this->shotgunSound = shotgunSound;
}

void Unit::setVelocity(float value) {
    velocity = value;
}

bool Unit::isMustBeDeleted() {
    return mustBeDeleted;
}

bool Unit::isGrounded() {
    return !falling && !jumping;
}

void Unit::resetJumpCount() {
    jumpCount = 0;
}

bool Unit::canJump() {
    return jumpCount < maxJumps && !jumping;
}
