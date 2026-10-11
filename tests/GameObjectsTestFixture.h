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
        go->gameOver = false;
    }

    void TearDown() override {
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
};

#endif /* GAMEOBJECTS_TEST_FIXTURE_H */
