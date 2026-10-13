#include <gtest/gtest.h>
#include "objects/interactive/Bullet.h"

TEST(BulletTest, SetDirectionRight) {
    Bullet b;
    b.setDirection("right");
}

TEST(BulletTest, SetExplosionSound) {
    Bullet b;
    b.setExplosionSound(nullptr);
    EXPECT_EQ(b.explosionSound, nullptr);
}

TEST(BulletTest, SetExplosionSoundToNonNull) {
    Bullet b;
    sf::SoundBuffer buf;
    sf::Sound sound(buf);
    b.setExplosionSound(&sound);
    EXPECT_NE(b.explosionSound, nullptr);
}
