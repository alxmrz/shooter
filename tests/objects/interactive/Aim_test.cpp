#include <gtest/gtest.h>
#include <cmath>
#include "fixtures/GameObjectsTestFixture.h"
#include "objects/interactive/Aim.h"

TEST_F(GameObjectsFixture, AimFollowsTarget) {
    Aim* aim = spawnAim(0, 0);

    aim->setTarget(250, 175);

    EXPECT_FLOAT_EQ(aim->getTargetX(), 250.f);
    EXPECT_FLOAT_EQ(aim->getTargetY(), 175.f);

    sf::Sprite* sprite = dynamic_cast<sf::Sprite*>(aim->getDrawForm());
    ASSERT_NE(sprite, nullptr);
    EXPECT_FLOAT_EQ(sprite->getPosition().x, 250.f);
    EXPECT_FLOAT_EQ(sprite->getPosition().y, 175.f);
}

TEST_F(GameObjectsFixture, AimDirectionPointsToTarget) {
    Aim* aim = spawnAim(0, 0);
    aim->setTarget(300, 400);

    sf::Vector2f direction = aim->getDirectionFrom(0, 0);

    EXPECT_NEAR(direction.x, 0.6f, 0.001f);
    EXPECT_NEAR(direction.y, 0.8f, 0.001f);
    EXPECT_NEAR(std::sqrt(direction.x * direction.x + direction.y * direction.y), 1.f, 0.001f);
}

TEST_F(GameObjectsFixture, AimDirectionIsBackwardWhenTargetBehind) {
    Aim* aim = spawnAim(0, 0);
    aim->setTarget(-100, 0);

    sf::Vector2f direction = aim->getDirectionFrom(0, 0);

    EXPECT_FLOAT_EQ(direction.x, -1.f);
    EXPECT_FLOAT_EQ(direction.y, 0.f);
}

TEST_F(GameObjectsFixture, AimCoincidentTargetHasNoDirection) {
    Aim* aim = spawnAim(100, 100);
    aim->setTarget(100, 100);

    sf::Vector2f direction = aim->getDirectionFrom(100, 100);

    EXPECT_FLOAT_EQ(direction.x, 0.f);
    EXPECT_FLOAT_EQ(direction.y, 0.f);
}
