#ifndef GAMEOBJECTS_TEST_FIXTURE_H
#define GAMEOBJECTS_TEST_FIXTURE_H

#include <gtest/gtest.h>
#include <new>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

#include "GameObjects.h"
#include "CObject.h"
#include "objects/interactive/Platform.h"
#include "objects/backgrounds/GroundBlock.h"
#include "objects/units/Shooter.h"
#include "objects/interactive/Bullet.h"
#include "objects/interactive/Aim.h"
#include "ui/Dialogue.h"

// Test double that counts how many times the game world updates it, so the
// dialogue pause can be observed without touching real objects.
class UpdateSpy : public CObject {
public:
    int updateCount = 0;

    UpdateSpy() : CObject(0, 0, 1, 1) {
    }

    void update(Event event) override {
        (void)event;
        updateCount++;
    }
};

// Test double that records the bullets a Shooter creates instead of using
// Fabric, so firing can be exercised without loading resources.
class RecordingShooter : public Shooter {
public:
    std::vector<Bullet*> spawned;

    RecordingShooter(GameObjects* go, float x, float y, int width, int height)
        : Shooter(go, x, y, width, height) {
        fireTime = 1.f;
    }

protected:
    Bullet* createBullet(float x, float y, int width, int height) override {
        Bullet* bullet = new Bullet(this, go, x, y, width, height);
        spawned.push_back(bullet);
        return bullet;
    }
};

// Shared DSL fixture for tests that need a live GameObjects container.
// It owns the raw storage for GameObjects (whose heavy members are placed
// manually, matching the rest of the suite) and provides builders so each
// test states only its case.
class GameObjectsFixture : public ::testing::Test {
protected:
    alignas(GameObjects) char goBuf[sizeof(GameObjects)];
    GameObjects* go = nullptr;

    sf::Texture dummyTexture;
    sf::Sprite mainSpr;
    sf::Sprite explosionSpr;

    static constexpr int ACTOR_W = 50;
    static constexpr int ACTOR_H = 80;

    void SetUp() override {
        dummyTexture.create(1, 1);
        mainSpr.setTexture(dummyTexture);
        explosionSpr.setTexture(dummyTexture);

        go = reinterpret_cast<GameObjects*>(goBuf);
        new (&go->all) std::vector<CObject*>();
        new (&go->borders) std::map<int, CObject*>();
        go->player = nullptr;
        go->fabric = nullptr;
        go->aim = nullptr;
        go->dialogue = nullptr;
        go->gameOver = false;
    }

    void TearDown() override {
        delete go->dialogue;
        for (CObject* object : go->all) {
            delete object;
        }
        go->all.~vector();
        go->borders.~map();
    }

    GroundBlock* placeGroundBlock(float x, float y, int width = 200, int height = 25) {
        GroundBlock* block = new GroundBlock(x, y, width, height);
        go->all.push_back(block);
        return block;
    }

    Shooter* registerActor(Shooter* actor) {
        actor->setMainSprite(&mainSpr);
        actor->setExplosionSprite(&explosionSpr);
        go->all.push_back(actor);
        return actor;
    }

    Shooter* spawnEnemy(float x, float y, int width = ACTOR_W, int height = ACTOR_H) {
        return registerActor(new Shooter(go, x, y, width, height));
    }

    template <typename T>
    T* spawnActor(float x, float y) {
        T* actor = new T(go, x, y, ACTOR_W, ACTOR_H);
        return static_cast<T*>(registerActor(actor));
    }

    Shooter* spawnPlayer(float x, float y, int width = ACTOR_W, int height = ACTOR_H) {
        Shooter* player = spawnEnemy(x, y, width, height);
        player->setMain(true);
        go->player = player;
        return player;
    }

    Aim* spawnAim(float x, float y) {
        Aim* aim = new Aim(go, x, y, 10, 10);
        aim->setMainSprite(makeSprite());
        aim->setTarget(x, y);
        go->aim = aim;
        go->all.push_back(aim);
        return aim;
    }

    RecordingShooter* spawnRecordingActor(float x, float y) {
        RecordingShooter* actor = new RecordingShooter(go, x, y, ACTOR_W, ACTOR_H);
        return static_cast<RecordingShooter*>(registerActor(actor));
    }

    RecordingShooter* spawnRecordingPlayer(float x, float y) {
        RecordingShooter* player = spawnRecordingActor(x, y);
        player->setMain(true);
        go->player = player;
        return player;
    }

    sf::Sprite* makeSprite() {
        return new sf::Sprite(dummyTexture);
    }

    Bullet* spawnBullet(float x, float y, Unit* shooter = nullptr, int width = 10, int height = 10) {
        Bullet* bullet = new Bullet(shooter, go, x, y, width, height);
        bullet->setMainSprite(makeSprite());
        bullet->setExplosionSprite(makeSprite());
        go->all.push_back(bullet);
        return bullet;
    }

    CObject* placeBorder(float x, float y, int width = 10, int height = 10) {
        CObject* border = new CObject(go, x, y, width, height);
        go->borders[(int)x] = border;
        go->all.push_back(border);
        return border;
    }

    void driveUpdates(Bullet* bullet, int count) {
        for (int i = 0; i < count; i++) {
            bullet->update(Event());
        }
    }

    bool containsObject(CObject* object) {
        for (CObject* item : go->all) {
            if (item == object) {
                return true;
            }
        }
        return false;
    }

    Dialogue* spawnDialogue(std::vector<std::string> fragments, float charsPerSecond = Dialogue::CHARS_PER_SECOND) {
        Dialogue* dialogue = new Dialogue(fragments, charsPerSecond);
        go->dialogue = dialogue;
        dialogue->open();
        return dialogue;
    }

    void updateDialogue(float dt) {
        go->dialogue->update(dt);
    }

    void advanceDialogue() {
        go->dialogue->advance();
    }

    bool dialogueOpen() {
        return go->dialogue != nullptr && go->dialogue->isOpen();
    }

    std::string dialogueVisibleText() {
        return go->dialogue->getVisibleText();
    }

    void expectDialogueText(const std::string& expected) {
        EXPECT_EQ(dialogueVisibleText(), expected);
    }

    void expectDialogueIndex(int expected) {
        EXPECT_EQ(go->dialogue->getCurrentIndex(), expected);
    }

    bool dialoguePromptShown() {
        return go->dialogue->promptShown();
    }

    UpdateSpy* spawnUpdateSpy() {
        UpdateSpy* spy = new UpdateSpy();
        go->all.push_back(spy);
        return spy;
    }

    void expectObjectNotUpdated(UpdateSpy* spy) {
        EXPECT_EQ(spy->updateCount, 0);
    }

    void expectObjectUpdated(UpdateSpy* spy) {
        EXPECT_EQ(spy->updateCount, 1);
    }
};

#endif /* GAMEOBJECTS_TEST_FIXTURE_H */
