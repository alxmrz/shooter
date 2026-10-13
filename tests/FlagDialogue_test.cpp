#include <gtest/gtest.h>

#include "GameObjectsTestFixture.h"

TEST_F(GameObjectsFixture, PlayerCapturesFlagOnTouch) {
    spawnPlayer(100.f, 100.f);
    spawnFlag(110.f, 100.f);

    touchFlag();

    expectFlagCount(1);
    EXPECT_FALSE(flagPresent());
}

TEST_F(GameObjectsFixture, PlayerCaptureOpensFlagDialogue) {
    spawnPlayer(100.f, 100.f);
    spawnFlag(110.f, 100.f);

    touchFlag();

    EXPECT_TRUE(flagDialogueOpen());
}

TEST_F(GameObjectsFixture, EnemyDoesNotCaptureFlag) {
    Shooter* enemy = spawnEnemy(100.f, 100.f);
    spawnFlag(110.f, 100.f);

    enemy->move(1.f, 0.f);

    expectFlagCount(0);
    EXPECT_TRUE(flagPresent());
}

TEST_F(GameObjectsFixture, ClosingFlagDialogueRestartsGame) {
    startFlagDialogue({"Hi"});
    updateDialogue(1.0f);

    advanceDialogue();

    EXPECT_FALSE(flagDialogueOpen());
    EXPECT_TRUE(consumeStageRestart());
}

TEST_F(GameObjectsFixture, SpaceDuringFlagTypingRevealsFragment) {
    startFlagDialogue({"Hello"});
    updateDialogue(0.1f);

    advanceDialogue();

    expectDialogueText("Hello");
    EXPECT_FALSE(consumeStageRestart());
}

TEST_F(GameObjectsFixture, WorldPausedDuringFlagDialogue) {
    UpdateSpy* spy = spawnUpdateSpy();
    startFlagDialogue({"Hello"});

    go->update(Event(), 0.1f);

    expectObjectNotUpdated(spy);
}

TEST_F(GameObjectsFixture, FirstStartShowsBaseIntro) {
    EXPECT_TRUE(introFragmentsContain(0, "Intro line"));
    EXPECT_FALSE(introFragmentsContain(0, "Flags"));
}

TEST_F(GameObjectsFixture, ContinuationReportsFlagsAndStage) {
    EXPECT_TRUE(introFragmentsContain(2, "Flags 2, stage 3"));
    EXPECT_FALSE(introFragmentsContain(2, "Intro line"));
}
