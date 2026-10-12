#include <gtest/gtest.h>
#include "GameObjectsTestFixture.h"

namespace {

void expectSpriteFrame(sf::Drawable* form, const sf::IntRect& expected) {
    sf::Sprite* sprite = dynamic_cast<sf::Sprite*>(form);
    ASSERT_NE(sprite, nullptr);
    EXPECT_EQ(sprite->getTextureRect(), expected);
}

}  // namespace

TEST_F(GameObjectsFixture, SetMainSpriteInitializesIdleFrame) {
    Shooter* enemy = spawnEnemy(100, 100);

    expectSpriteFrame(enemy->getDrawForm(), sf::IntRect(24, 144, 50, 50));
}

TEST_F(GameObjectsFixture, MovingFrameShownBeforeInterval) {
    Shooter* enemy = spawnEnemy(100, 100);
    enemy->move("right");

    enemy->refreshSpriteFrame(0.f);

    expectSpriteFrame(enemy->getDrawForm(), sf::IntRect(26, 315, 50, 50));
}

TEST_F(GameObjectsFixture, MovingFrameAdvancesAfterInterval) {
    Shooter* enemy = spawnEnemy(100, 100);
    enemy->move("right");

    enemy->refreshSpriteFrame(0.08f);

    expectSpriteFrame(enemy->getDrawForm(), sf::IntRect(76, 315, 50, 50));
}

TEST_F(GameObjectsFixture, IdleFrameStableOnRefresh) {
    Shooter* enemy = spawnEnemy(100, 100);

    enemy->refreshSpriteFrame(0.08f);
    enemy->refreshSpriteFrame(0.08f);

    expectSpriteFrame(enemy->getDrawForm(), sf::IntRect(24, 144, 50, 50));
}
