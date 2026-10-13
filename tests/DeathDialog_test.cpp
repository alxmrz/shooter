#include <gtest/gtest.h>

#include "GameObjectsTestFixture.h"

// Retry after death resets the run: captured flags go back to zero and the run
// restarts from stage 1.
TEST_F(GameObjectsFixture, ResetProgressClearsFlags) {
    go->gs->collectFlag();
    go->gs->collectFlag();
    expectFlagCount(2);
    expectStage(3);

    go->gs->resetProgress();

    expectFlagCount(0);
    expectStage(1);
}
