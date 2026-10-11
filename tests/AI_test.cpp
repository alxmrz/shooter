#include <gtest/gtest.h>
#include "GameObjectsTestFixture.h"
#include "AI.h"
#include "objects/units/Shooter.h"

// Test double: records attack() calls instead of spawning bullets, so the AI
// can be exercised without a live Fabric/resources.
class CountingShooter : public Shooter {
public:
    CountingShooter(GameObjects* go, float x, float y, int width, int height)
        : Shooter(go, x, y, width, height) {}

    int attacks = 0;

    void attack() override {
        attacks++;
    }
};

// --- Solid-surface query (ledge detection) ---

TEST_F(GameObjectsFixture, HasSupportAhead) {
    placeGroundBlock(100, 300, 600);
    Shooter* enemy = spawnEnemy(150, 300 - ACTOR_H);
    AI ai(go, enemy);

    EXPECT_TRUE(ai.hasSupportAhead("right"));
}

TEST_F(GameObjectsFixture, HasSupportAheadPastEdge) {
    placeGroundBlock(100, 300, 200);
    Shooter* enemy = spawnEnemy(260, 300 - ACTOR_H);
    AI ai(go, enemy);

    EXPECT_FALSE(ai.hasSupportAhead("right"));
}

// --- Perception ---

TEST_F(GameObjectsFixture, EnemyDetection) {
    placeGroundBlock(100, 300, 900);
    Shooter* enemy = spawnEnemy(150, 300 - ACTOR_H);
    Shooter* player = spawnPlayer(150 + 400, 300 - ACTOR_H);
    AI ai(go, enemy);

    EXPECT_FALSE(ai.detectsPlayer());

    player->setCoords(200, 300 - ACTOR_H - 100);
    EXPECT_FALSE(ai.detectsPlayer());
}

// --- Patrolling ---

TEST_F(GameObjectsFixture, EnemyReversesAtRightEdge) {
    placeGroundBlock(100, 300, 200);
    Shooter* enemy = spawnEnemy(260, 300 - ACTOR_H);
    spawnPlayer(2500, 2500);
    AI ai(go, enemy);

    ai.think();

    EXPECT_LT(enemy->getX(), 260.f);
}

TEST_F(GameObjectsFixture, EnemyReversesAtLeftEdge) {
    placeGroundBlock(100, 300, 200);
    Shooter* enemy = spawnEnemy(90, 300 - ACTOR_H);
    spawnPlayer(2500, 2500);
    AI ai(go, enemy);
    ai.setPatrolDirection("left");

    ai.think();

    EXPECT_GT(enemy->getX(), 90.f);
}

// --- Engagement ---

TEST_F(GameObjectsFixture, EnemyApproachesPlayer) {
    placeGroundBlock(100, 300, 600);
    Shooter* enemy = spawnEnemy(150, 300 - ACTOR_H);
    spawnPlayer(300, 300 - ACTOR_H);
    AI ai(go, enemy);

    ai.think();

    EXPECT_GT(enemy->getX(), 150.f);
}

TEST_F(GameObjectsFixture, EnemyStopsAt100) {
    placeGroundBlock(100, 300, 600);
    CountingShooter* enemy = spawnActor<CountingShooter>(180, 300 - ACTOR_H);
    spawnPlayer(260, 300 - ACTOR_H);
    AI ai(go, enemy);

    ai.think();

    EXPECT_FLOAT_EQ(enemy->getX(), 180.f);
    EXPECT_EQ(enemy->attacks, 1);
}

TEST_F(GameObjectsFixture, EnemyDoesNotStepOffLedge) {
    placeGroundBlock(100, 300, 200);
    CountingShooter* enemy = spawnActor<CountingShooter>(260, 300 - ACTOR_H);
    spawnPlayer(500, 300 - ACTOR_H);
    AI ai(go, enemy);

    ai.think();

    EXPECT_FLOAT_EQ(enemy->getX(), 260.f);
    EXPECT_EQ(enemy->attacks, 1);
}
