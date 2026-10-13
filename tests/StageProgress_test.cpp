#include <gtest/gtest.h>

#include "StageProgress.h"

TEST(StageProgressTest, StageStartsAtStageOneAndZeroFlags) {
    StageProgress progress;
    EXPECT_EQ(progress.getStage(), 1);
    EXPECT_EQ(progress.getFlags(), 0);
}

TEST(StageProgressTest, CollectFlagIncrementsFlagsAndAdvancesStage) {
    StageProgress progress;
    progress.collectFlag();
    EXPECT_EQ(progress.getFlags(), 1);
    EXPECT_EQ(progress.getStage(), 2);
    progress.collectFlag();
    EXPECT_EQ(progress.getFlags(), 2);
    EXPECT_EQ(progress.getStage(), 3);
}

TEST(StageProgressTest, FlagsAccumulateAcrossStages) {
    StageProgress progress;
    progress.collectFlag();
    progress.collectFlag();
    EXPECT_EQ(progress.getFlags(), 2);
    EXPECT_EQ(progress.getStage(), 3);
}

TEST(StageProgressTest, ResetClearsProgress) {
    StageProgress progress;
    progress.collectFlag();
    progress.collectFlag();
    progress.reset();
    EXPECT_EQ(progress.getStage(), 1);
    EXPECT_EQ(progress.getFlags(), 0);
}
