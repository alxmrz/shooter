#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Unit.h"
#include "../interactive/Crystal.h"
#include "../interactive/Heart.h"
#include "../interactive/Ammunition.h"
#include "../interactive/Platform.h"
#include "../interactive/Flag.h"
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

    collectFromWorld(0);
}

void Unit::collectFromWorld(unsigned i) {
    if (i >= go->all.size()) {
        return;
    }

    collectAtOrContinue(i);
}

void Unit::collectAtOrContinue(unsigned i) {
    if (collectAt(i)) {
        return;
    }

    collectFromWorld(i + 1);
}

bool Unit::collectAt(unsigned i) {
    CObject* object = go->all[i];

    return tryCollectCrystal(object, i) ||
           tryCollectAmmo(object, i) ||
           tryCollectHeart(object, i) ||
           tryCollectFlagObject(object, i);
}

bool Unit::tryCollectCrystal(CObject* object, unsigned index) {
    Crystal* crystal = dynamic_cast<Crystal*>(object);

    if (crystal == nullptr || !collideRect(crystal)) {
        return false;
    }

    crystals++;
    crystalSound->play();
    go->all.erase(go->all.begin() + index);
    return true;
}

bool Unit::tryCollectAmmo(CObject* object, unsigned index) {
    Ammunition* ammoObject = dynamic_cast<Ammunition*>(object);

    if (ammoObject == nullptr || !canCollectAmmo(ammoObject)) {
        return false;
    }

    collectAmmo(index);
    return true;
}

bool Unit::canCollectAmmo(Ammunition* ammoObject) {
    return collideRect(ammoObject) && getAmmo() < 10;
}

void Unit::collectAmmo(unsigned index) {
    ammo += 3;
    clampAmmo();
    go->all.erase(go->all.begin() + index);
}

void Unit::clampAmmo() {
    if (ammo > 10) {
        ammo = 10;
    }
}

bool Unit::tryCollectHeart(CObject* object, unsigned index) {
    Heart* heart = dynamic_cast<Heart*>(object);

    if (heart == nullptr || !canCollectHeart(heart)) {
        return false;
    }

    health++;
    go->all.erase(go->all.begin() + index);
    return true;
}

bool Unit::canCollectHeart(Heart* heart) {
    return collideRect(heart) && getHealth() < 3;
}

bool Unit::tryCollectFlagObject(CObject* object, unsigned index) {
    Flag* flag = dynamic_cast<Flag*>(object);

    if (flag == nullptr) {
        return false;
    }

    return tryCollectFlag(flag, index);
}

bool Unit::tryCollectFlag(Flag* flag, unsigned index) {
    if (!collideRect(flag)) {
        return false;
    }

    go->collectFlag();
    go->all.erase(go->all.begin() + index);
    return true;
}

bool Unit::resolvePlatformCollision(float x, float y) {
    Platform* platform = findCollidingPlatform();
    if (platform == nullptr) {
        return true;
    }

    return applyPlatformCollision(platform, x, y);
}

Platform* Unit::findCollidingPlatform() {
    std::vector<CObject*>::iterator it =
        std::find_if(go->all.begin(), go->all.end(), SolidPlatformFinder{this});

    if (it == go->all.end()) {
        return nullptr;
    }

    return dynamic_cast<Platform*>(*it);
}

bool Unit::SolidPlatformFinder::operator()(CObject* object) const {
    return unit->isSolidCollision(dynamic_cast<Platform*>(object));
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
    if (!canStartJump()) {
        return;
    }

    initiateJump();
}

bool Unit::canStartJump() {
    return !jumping && canJump();
}

void Unit::initiateJump() {
    jumpCount++;
    beginJumpIfNotStarted();
    velocityHorizontal = -1.0f;
    jumping = true;
}

void Unit::beginJumpIfNotStarted() {
    if (currentJumpHeight == 0.f) {
        applyJumpImpulse();
    }
}

void Unit::applyJumpImpulse() {
    onSecondJumpIfNeeded();
    jumpSound->play();
    currentJumpHeight = this->y - jumpHeightFor(jumpCount);
}

float Unit::jumpHeightFor(int count) {
    if (count == 2) {
        return 60.f;
    }

    return 100.f;
}

void Unit::onSecondJumpIfNeeded() {
    if (jumpCount == 2) {
        onSecondJump();
    }
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
    handleRemoval();
    think();
    updateVerticalMotion();
    updatePlayerInput(event);
}

void Unit::handleRemoval() {
    if (remove()) {
        destroyIfAllowed();
    }
}

void Unit::destroyIfAllowed() {
    // Не удаляем игрока, пока не показан экран завершения игры
    if (!(isPlayer() && !go->gameOver)) {
        destroy();
    }
}

void Unit::updateVerticalMotion() {
    if (jumping) {
        updateJump();
        return;
    }

    gravitate();
}

void Unit::updateJump() {
    adjustJumpVelocity();
    continueOrStopJump();
}

void Unit::adjustJumpVelocity() {
    if (velocityHorizontal > -maxVelocity) {
        velocityHorizontal -= acceleration;
    }
}

void Unit::continueOrStopJump() {
    if (this->y > currentJumpHeight && move(0.f, velocityHorizontal)) {
        return;
    }

    stopJump();
}

void Unit::stopJump() {
    jumping = false;
    velocityHorizontal = 0.f;
    currentJumpHeight = 0.f;
}

void Unit::updatePlayerInput(Event event) {
    if (!isPlayer()) {
        return;
    }

    handleReleasedKeys(event);
    handlePlatformJump();
}

void Unit::handleReleasedKeys(Event event) {
    if (!event.IsKeyReleased()) {
        return;
    }

    stopMovingIfHorizontalReleased(event);
}

void Unit::stopMovingIfHorizontalReleased(Event event) {
    if (isHorizontalReleased(event)) {
        stopMoving();
    }
}

bool Unit::isHorizontalReleased(Event event) {
    return event.IsKeyBoardLeft() || event.IsKeyBoardRight();
}

void Unit::handlePlatformJump() {
    if (!isPlatformAffected) {
        return;
    }

    jump();
    clearPlatformAffected();
}

void Unit::clearPlatformAffected() {
    if (!isJumping()) {
        isPlatformAffected = false;
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
