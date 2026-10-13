#include <gtest/gtest.h>
#include <cmath>
#include <vector>

struct CollisionObject {
    float x;
    float y;
    int width;
    int height;
    std::string type;
};

struct MoveResult {
    float finalX;
    float finalY;
    bool stoppedFall;
    bool stoppedJump;
};

static bool rectsOverlap(const CollisionObject& a, const CollisionObject& b) {
    return a.x < b.x + b.width &&
           a.x + a.width > b.x &&
           a.y < b.y + b.height &&
           a.y + a.height > b.y;
}

static bool pointInRect(float px, float py, const CollisionObject& r) {
    return px >= r.x && px <= r.x + r.width &&
           py >= r.y && py <= r.y + r.height;
}

static bool collideRect(const CollisionObject& a, const CollisionObject& b) {
    if (pointInRect(a.x, a.y, b)) return true;
    if (pointInRect(a.x + a.width, a.y, b)) return true;
    if (pointInRect(a.x, a.y + a.height, b)) return true;
    if (pointInRect(a.x + a.width, a.y + a.height, b)) return true;
    if (pointInRect(b.x, b.y, a)) return true;
    if (pointInRect(b.x + b.width, b.y, a)) return true;
    if (pointInRect(b.x, b.y + b.height, a)) return true;
    if (pointInRect(b.x + b.width, b.y + b.height, a)) return true;
    return false;
}

static MoveResult simulatePlatformCollision(
    float playerX, float playerY, int playerW, int playerH,
    float moveX, float moveY,
    float platX, float platY, int platW, int platH,
    bool jumping
) {
    CollisionObject player = {playerX, playerY, playerW, playerH};
    CollisionObject platform = {platX, platY, platW, platH};

    float newX = player.x + moveX;
    float newY = player.y + moveY;
    CollisionObject movedPlayer = {newX, newY, playerW, playerH};

    MoveResult result = {newX, newY, false, false};

    if (!collideRect(movedPlayer, platform)) {
        return result;
    }

    if (moveY > 0) {
        result.finalY = platform.y - playerH;
        result.stoppedFall = true;
    } else if (moveY < 0) {
        result.stoppedJump = true;
        result.finalY = newY;
    } else if (moveX != 0) {
        result.finalX = player.x;
        result.finalY = player.y;
    }

    return result;
}

class PlatformCollisionTest : public ::testing::Test {
protected:
    const int PLAYER_W = 50;
    const int PLAYER_H = 80;
    const int PLAT_W = 200;
    const int PLAT_H = 25;
};

TEST_F(PlatformCollisionTest, FallingDown_LandsOnPlatform) {
    float platY = 500.f;
    MoveResult r = simulatePlatformCollision(
        100.f, 420.f, PLAYER_W, PLAYER_H,
        0.f, 20.f,
        50.f, platY, PLAT_W, PLAT_H,
        false
    );

    EXPECT_TRUE(r.stoppedFall);
    EXPECT_FALSE(r.stoppedJump);
    EXPECT_FLOAT_EQ(r.finalY, platY - PLAYER_H);
}

TEST_F(PlatformCollisionTest, JumpingUp_HitsPlatformFromBelow) {
    float platY = 300.f;
    MoveResult r = simulatePlatformCollision(
        100.f, 320.f, PLAYER_W, PLAYER_H,
        0.f, -15.f,
        50.f, platY, PLAT_W, PLAT_H,
        true
    );

    EXPECT_FALSE(r.stoppedFall);
    EXPECT_TRUE(r.stoppedJump);
}

TEST_F(PlatformCollisionTest, HorizontalMove_DoesNotSnapToTop) {
    float playerY = 400.f;
    MoveResult r = simulatePlatformCollision(
        30.f, playerY, PLAYER_W, PLAYER_H,
        10.f, 0.f,
        50.f, 420.f, PLAT_W, PLAT_H,
        false
    );

    EXPECT_FALSE(r.stoppedFall);
    EXPECT_FALSE(r.stoppedJump);
    EXPECT_FLOAT_EQ(r.finalY, playerY);
}

TEST_F(PlatformCollisionTest, FallingThroughPlatform_SnapsToCorrectY) {
    float platY = 600.f;
    MoveResult r = simulatePlatformCollision(
        100.f, 520.f, PLAYER_W, PLAYER_H,
        0.f, 20.f,
        50.f, platY, PLAT_W, PLAT_H,
        false
    );

    EXPECT_TRUE(r.stoppedFall);
    EXPECT_FLOAT_EQ(r.finalY, platY - PLAYER_H);
}

TEST_F(PlatformCollisionTest, JumpingUp_DoesNotSnapToTop) {
    float platY = 300.f;
    MoveResult r = simulatePlatformCollision(
        100.f, 320.f, PLAYER_W, PLAYER_H,
        0.f, -15.f,
        50.f, platY, PLAT_W, PLAT_H,
        true
    );

    EXPECT_NE(r.finalY, platY - PLAYER_H);
}

TEST_F(PlatformCollisionTest, HorizontalMove_PlayerStaysAtSameY) {
    float playerY = 400.f;
    MoveResult r = simulatePlatformCollision(
        30.f, playerY, PLAYER_W, PLAYER_H,
        10.f, 0.f,
        50.f, 410.f, PLAT_W, PLAT_H,
        false
    );

    EXPECT_FLOAT_EQ(r.finalY, playerY);
    EXPECT_FALSE(r.stoppedFall);
}

TEST_F(PlatformCollisionTest, FallingOntoPlatform_SetsCorrectPosition) {
    float platY = 800.f;
    MoveResult r = simulatePlatformCollision(
        200.f, 750.f, PLAYER_W, PLAYER_H,
        0.f, 60.f,
        150.f, platY, PLAT_W, PLAT_H,
        false
    );

    EXPECT_TRUE(r.stoppedFall);
    EXPECT_FLOAT_EQ(r.finalY, platY - PLAYER_H);
}

TEST_F(PlatformCollisionTest, MovingLeftIntoPlatform_DoesNotSnap) {
    float playerY = 400.f;
    MoveResult r = simulatePlatformCollision(
        280.f, playerY, PLAYER_W, PLAYER_H,
        -10.f, 0.f,
        200.f, 420.f, PLAT_W, PLAT_H,
        false
    );

    EXPECT_FALSE(r.stoppedFall);
    EXPECT_FLOAT_EQ(r.finalY, playerY);
}
