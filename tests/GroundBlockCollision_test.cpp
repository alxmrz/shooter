#include <gtest/gtest.h>
#include <new>
#include "objects/units/Shooter.h"
#include "objects/backgrounds/GroundBlock.h"

class GroundBlockCollisionTest : public ::testing::Test {
protected:
    alignas(GameObjects) char goBuf[sizeof(GameObjects)];
    GameObjects* go;

    sf::Texture dummyTexture;
    sf::Sprite mainSpr;
    sf::Sprite explosionSpr;

    const int SW = 50;
    const int SH = 80;
    const int BW = 200;
    const int BH = 25;

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
        go->all.~vector();
        go->borders.~map();
    }

    Shooter* createPlayer(float x, float y) {
        Shooter* s = new Shooter(go, x, y, SW, SH);
        s->setMain(true);
        s->setMainSprite(&mainSpr);
        s->setExplosionSprite(&explosionSpr);
        return s;
    }
};

TEST_F(GroundBlockCollisionTest, FallingOntoBlock_SnapsToTop) {
    // Block: x=[100,300] y=[200,225]
    // Shooter: x=[100,150] y=[100,180] — falls down onto the block
    GroundBlock* block = new GroundBlock(100, 200, BW, BH);
    go->all.push_back(block);

    Shooter* shooter = createPlayer(100, 100);

    shooter->move(0.f, 40.f);

    EXPECT_FLOAT_EQ(shooter->getY(), block->getY() - SH);

    delete shooter;
    go->all.clear();
    delete block;
}

TEST_F(GroundBlockCollisionTest, MoveRightIntoBlock_XUnchanged) {
    // Block: x=[140,340] y=[100,125]
    // Shooter: x=[130,180] y=[110,190] — overlaps from the left
    GroundBlock* block = new GroundBlock(140, 100, BW, BH);
    go->all.push_back(block);

    Shooter* shooter = createPlayer(130, 110);
    float startX = shooter->getX();
    float startY = shooter->getY();

    shooter->move(10.f, 0.f);

    EXPECT_FLOAT_EQ(shooter->getX(), startX);
    EXPECT_FLOAT_EQ(shooter->getY(), startY);

    delete shooter;
    go->all.clear();
    delete block;
}

TEST_F(GroundBlockCollisionTest, MoveLeftIntoBlock_XUnchanged) {
    // Block: x=[160,360] y=[100,125]
    // Shooter: x=[210,260] y=[110,190] — overlaps from the right
    GroundBlock* block = new GroundBlock(160, 100, BW, BH);
    go->all.push_back(block);

    Shooter* shooter = createPlayer(210, 110);
    float startX = shooter->getX();

    shooter->move(-10.f, 0.f);

    EXPECT_FLOAT_EQ(shooter->getX(), startX);

    delete shooter;
    go->all.clear();
    delete block;
}

TEST_F(GroundBlockCollisionTest, MoveUpIntoBlockFromBelow_YUnchanged) {
    // Block: x=[100,300] y=[100,125]
    // Shooter: x=[100,150] y=[115,195] — overlaps from below
    GroundBlock* block = new GroundBlock(100, 100, BW, BH);
    go->all.push_back(block);

    Shooter* shooter = createPlayer(100, 115);
    float startY = shooter->getY();

    shooter->move(0.f, -10.f);

    EXPECT_FLOAT_EQ(shooter->getY(), startY);

    delete shooter;
    go->all.clear();
    delete block;
}

TEST_F(GroundBlockCollisionTest, MoveWithoutOverlap_ChangesPosition) {
    GroundBlock* block = new GroundBlock(400, 100, BW, BH);
    go->all.push_back(block);

    Shooter* shooter = createPlayer(10, 200);
    float startX = shooter->getX();

    shooter->move(10.f, 0.f);

    EXPECT_FLOAT_EQ(shooter->getX(), startX + 10.f);

    delete shooter;
    go->all.clear();
    delete block;
}

TEST_F(GroundBlockCollisionTest, BlockStaysStaticAfterCollision) {
    GroundBlock* block = new GroundBlock(140, 100, BW, BH);
    go->all.push_back(block);

    float blockX = block->getX();
    float blockY = block->getY();
    int blockW = block->getWidth();
    int blockH = block->getHeight();

    Shooter* shooter = createPlayer(130, 110);
    shooter->move(10.f, 0.f);

    EXPECT_FLOAT_EQ(block->getX(), blockX);
    EXPECT_FLOAT_EQ(block->getY(), blockY);
    EXPECT_EQ(block->getWidth(), blockW);
    EXPECT_EQ(block->getHeight(), blockH);

    delete shooter;
    go->all.clear();
    delete block;
}
