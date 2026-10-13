#include <gtest/gtest.h>
#include <new>
#include "objects/units/Shooter.h"
#include "objects/interactive/Platform.h"

class ShooterPlatformCollisionTest : public ::testing::Test {
protected:
    alignas(GameObjects) char goBuf[sizeof(GameObjects)];
    GameObjects* go;

    sf::Texture dummyTexture;
    sf::Sprite mainSpr;
    sf::Sprite explosionSpr;

    const int SW = 50;
    const int SH = 80;
    const int PW = 200;
    const int PH = 25;

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

TEST_F(ShooterPlatformCollisionTest, MoveRightIntoPlatform_XUnchanged) {
    // Platform: x=[140,340] y=[100,125]
    // Shooter:  x=[130,180] y=[110,190] — overlaps from left
    Platform* plat = new Platform(140, 100, PW, PH);
    go->all.push_back(plat);

    Shooter* shooter = createPlayer(130, 110);
    float startX = shooter->getX();
    float startY = shooter->getY();

    shooter->move(10.f, 0.f);

    EXPECT_FLOAT_EQ(shooter->getX(), startX);
    EXPECT_FLOAT_EQ(shooter->getY(), startY);

    delete shooter;
    go->all.clear();
    delete plat;
}

TEST_F(ShooterPlatformCollisionTest, MoveLeftIntoPlatform_XUnchanged) {
    // Platform: x=[160,360] y=[100,125]
    // Shooter:  x=[210,260] y=[110,190] — overlaps from right
    Platform* plat = new Platform(160, 100, PW, PH);
    go->all.push_back(plat);

    Shooter* shooter = createPlayer(210, 110);
    float startX = shooter->getX();
    float startY = shooter->getY();

    shooter->move(-10.f, 0.f);

    EXPECT_FLOAT_EQ(shooter->getX(), startX);
    EXPECT_FLOAT_EQ(shooter->getY(), startY);

    delete shooter;
    go->all.clear();
    delete plat;
}

TEST_F(ShooterPlatformCollisionTest, MoveUpIntoPlatformFromBelow_YUnchanged) {
    // Platform: x=[100,300] y=[100,125]
    // Shooter:  x=[100,150] y=[115,195] — overlaps from below
    Platform* plat = new Platform(100, 100, PW, PH);
    go->all.push_back(plat);

    Shooter* shooter = createPlayer(100, 115);
    float startX = shooter->getX();
    float startY = shooter->getY();

    shooter->move(0.f, -10.f);

    EXPECT_FLOAT_EQ(shooter->getX(), startX);
    EXPECT_FLOAT_EQ(shooter->getY(), startY);

    delete shooter;
    go->all.clear();
    delete plat;
}

TEST_F(ShooterPlatformCollisionTest, MoveRightNoOverlap_XChanges) {
    Platform* plat = new Platform(400, 100, PW, PH);
    go->all.push_back(plat);

    Shooter* shooter = createPlayer(10, 200);
    float startX = shooter->getX();

    shooter->move(10.f, 0.f);

    EXPECT_FLOAT_EQ(shooter->getX(), startX + 10.f);

    delete shooter;
    go->all.clear();
    delete plat;
}

TEST_F(ShooterPlatformCollisionTest, MoveLeftNoOverlap_XChanges) {
    Platform* plat = new Platform(10, 100, PW, PH);
    go->all.push_back(plat);

    Shooter* shooter = createPlayer(400, 200);
    float startX = shooter->getX();

    shooter->move(-10.f, 0.f);

    EXPECT_FLOAT_EQ(shooter->getX(), startX - 10.f);

    delete shooter;
    go->all.clear();
    delete plat;
}

TEST_F(ShooterPlatformCollisionTest, MoveUpNoOverlap_YChanges) {
    Platform* plat = new Platform(100, 200, PW, PH);
    go->all.push_back(plat);

    Shooter* shooter = createPlayer(100, 20);
    float startY = shooter->getY();

    shooter->move(0.f, -10.f);

    EXPECT_FLOAT_EQ(shooter->getY(), startY - 10.f);

    delete shooter;
    go->all.clear();
    delete plat;
}

TEST_F(ShooterPlatformCollisionTest, CollideRectFromRight) {
    Platform* plat = new Platform(140, 100, PW, PH);
    Shooter* shooter = createPlayer(130, 110);
    EXPECT_TRUE(shooter->collideRect(plat));
    delete shooter;
    delete plat;
}

TEST_F(ShooterPlatformCollisionTest, CollideRectFromLeft) {
    Platform* plat = new Platform(160, 100, PW, PH);
    Shooter* shooter = createPlayer(210, 110);
    EXPECT_TRUE(shooter->collideRect(plat));
    delete shooter;
    delete plat;
}

TEST_F(ShooterPlatformCollisionTest, CollideRectFromBottom) {
    Platform* plat = new Platform(100, 100, PW, PH);
    Shooter* shooter = createPlayer(100, 115);
    EXPECT_TRUE(shooter->collideRect(plat));
    delete shooter;
    delete plat;
}

TEST_F(ShooterPlatformCollisionTest, CollideRectNoOverlap) {
    Platform* plat = new Platform(400, 100, PW, PH);
    Shooter* shooter = createPlayer(10, 200);
    EXPECT_FALSE(shooter->collideRect(plat));
    delete shooter;
    delete plat;
}
