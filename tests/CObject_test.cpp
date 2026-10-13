#include <gtest/gtest.h>
#include "fixtures/GameObjectsTestFixture.h"
#include "CObject.h"

class CollisionObjectTest : public GameObjectsFixture {
protected:
    CObject* spawnObject(float x, float y, int width = 10, int height = 10) {
        CObject* object = new CObject(go, x, y, width, height);
        go->all.push_back(object);
        return object;
    }
};

TEST_F(CollisionObjectTest, CollidePointInsideIsTrue) {
    CObject* object = spawnObject(0, 0, 10, 10);

    EXPECT_TRUE(object->collidePoint(5, 5));
}

TEST_F(CollisionObjectTest, CollidePointOnOwnEdgeIsFalse) {
    CObject* object = spawnObject(0, 0, 10, 10);

    EXPECT_FALSE(object->collidePoint(0, 5));
}

TEST_F(CollisionObjectTest, CollidePointOutsideIsFalse) {
    CObject* object = spawnObject(0, 0, 10, 10);

    EXPECT_FALSE(object->collidePoint(50, 50));
}

TEST_F(CollisionObjectTest, CollideRectOverlappingIsTrue) {
    CObject* first = spawnObject(0, 2, 10, 10);
    CObject* second = spawnObject(4, 2, 10, 10);

    EXPECT_TRUE(first->collideRect(second));
    EXPECT_TRUE(second->collideRect(first));
}

TEST_F(CollisionObjectTest, CollideRectSeparatedIsFalse) {
    CObject* first = spawnObject(0, 2, 10, 10);
    CObject* second = spawnObject(100, 2, 10, 10);

    EXPECT_FALSE(first->collideRect(second));
}

TEST_F(CollisionObjectTest, CollideObjectAfterMoveHitsBorder) {
    CObject* mover = spawnObject(0, 2, 10, 10);
    placeBorder(8, 2, 10, 10);

    EXPECT_TRUE(mover->collideObjectAfterMove(5.f, 0.f));
}

TEST_F(CollisionObjectTest, CollideObjectAfterMoveMissesDistantBorder) {
    CObject* mover = spawnObject(0, 2, 10, 10);
    placeBorder(100, 2, 10, 10);

    EXPECT_FALSE(mover->collideObjectAfterMove(5.f, 0.f));
}

TEST_F(CollisionObjectTest, DestroyRemovesObjectFromWorld) {
    CObject* object = spawnObject(0, 0, 10, 10);
    ASSERT_TRUE(containsObject(object));

    object->destroy();

    EXPECT_FALSE(containsObject(object));
}
